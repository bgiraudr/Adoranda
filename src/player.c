#include <gint/keyboard.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "player.h"
#include "define.h"
#include "map.h"
#include "stats.h"
#include "capacite.h"
#include "util.h"
#include "talkable.h"
#include "draw_util.h"
#include "animation.h"

extern struct LevelUpPlayer levelupplayer;
extern bopti_image_t img_dialogue;

struct Player init_player(void) {

	struct Stats bstats = {
		.atk = 15,
		.def = 15,
		.spe_atk = 15,
		.spe_def = 15,
		.level = 1,
		.pv = 30,
	};

	struct Stats stats = {
		.level = 1,
		.xp = 0,
		.type = "Rédacteur",
	};

	struct Inventory inventory = {
		.nbItems = 0
	};

	struct Player player = {
		.pos = VEC2(7, 8),
		.pos_visual = VEC2F(7*TILE_SIZE, 8*TILE_SIZE),
		.base_stats = bstats,
		.stats = stats,
		.inventory = inventory,
		.x_mid = 6,
		.y_mid = 1,
		.show_x = 12,
		.show_y = 7,
		.direction = DIR_DOWN,
		.anim.function = anim_player_idle,
		.anim.dir = DIR_DOWN
	};
	player.idle = !anim_player_idle(&player.anim, 1);
	player.moves[0] = copy_move(get_move_id(1));
	player.moves[1] = copy_move(get_move_id(4));

	set_stats_level_from(&player.base_stats, &player.stats);
	player.stats.pv = player.stats.max_pv;
	return player;
}

void check_level(struct Player *player, int prec_level) {
	for(int i = 0; i < levelupplayer.nbLevelUp; i++) {
		if(player->stats.level >= levelupplayer.levelup[i]->level
		 && prec_level < levelupplayer.levelup[i]->level) {
			add_move(player, get_move_id(levelupplayer.levelup[i]->id_move));
		}
	}
}

/*
return the info tile value the player is facing to
TILE_SOLID by default (out of bound)
*/
int player_facing(struct Game const *game) {
	int direction = game->player->direction;
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	int index = game->player->pos.x + dx + game->map->w * (game->player->pos.y + dy);
	if(game->player->pos.x + dx >= 0 &&
		game->player->pos.x + dx <= game->map->w &&
		game->player->pos.y + dy >= 0 &&
		game->player->pos.y + dy <= game->map->h) {
		return game->map->info_map[index];
	}
	return TILE_SOLID;
}

int get_nb_moves(struct Player *player) {
	for(int i = 0; i < NB_PLAYER_MOVES; i++) {
		if(player->moves[i] == NULL) {
			return i;
		}
	}
	return NB_PLAYER_MOVES;
}

bool has_move(struct Player *player, struct Move move) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		if(player->moves[i]->id == move.id) return true;
	}
	return false;
}

void add_move(struct Player *player, struct Move move) {
	int index = get_nb_moves(player);
	if(!has_move(player, move)) {
		if(index != NB_PLAYER_MOVES) {
			draw_text(50, DHEIGHT-47, C_BLACK, "Vous apprenez %s !", move.name);
			dupdate();
			wait_for_input(KEY_SHIFT);
			player->moves[index] = copy_move(move);
		} else {
			draw_text(50, DHEIGHT-47, C_BLACK, "Vous pouvez apprendre %s !", move.name);
			dupdate();
			wait_for_input(KEY_SHIFT);
			replace_capacities(player, move);
		}
	} else {
		draw_text(50, DHEIGHT-47, C_BLACK, "Vous connaissez déjà la capacité %s !", move.name);
		dupdate();
		wait_for_input(KEY_SHIFT);
	}
}

void draw_player_moves(struct Player *player) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		draw_classic_move(20,65*i+15,player->moves[i]);
	}
}

void replace_capacities(struct Player *player, struct Move move) {
	int selection = 0;
	int buffer = keydown(KEY_SHIFT);
	while(1) {
		clearevents();
		dclear(C_WHITE);

		selection += keydown(KEY_DOWN) - keydown(KEY_UP);
		if(selection > NB_PLAYER_MOVES-1) selection = NB_PLAYER_MOVES-1;
		if(selection < 0) selection = 0;

		dtext(180,15,C_BLACK, "Remplacer");
		draw_classic_move(200,DHEIGHT/2-30, &move);
		draw_player_moves(player);
		
		dtext(105, 42+65*selection , C_RED, "[X]");
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		if(keydown(KEY_EXIT)) {
			selection = -1;
			break;
		}
		while(keydown_any(KEY_DOWN,KEY_UP, KEY_SHIFT,0)) clearevents();
	}
	if(selection >= 0) {
		free(player->moves[selection]);
		player->moves[selection] = copy_move(move);
	}
}

int select_capacity(struct Player *player, char* context, bool allow_back) {
	int selection = 0;
	int buffer = keydown(KEY_SHIFT);
	while(1) {
		clearevents();
		dclear(C_WHITE);

		selection += keydown(KEY_DOWN) - keydown(KEY_UP);
		if(selection > NB_PLAYER_MOVES-1) selection = NB_PLAYER_MOVES-1;
		if(selection < 0) selection = 0;

		dtext(180,10,C_BLACK,context);
		draw_player_moves(player);
		
		dtext(105, 42+65*selection , C_RED, "[X]");
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		if(keydown(KEY_EXIT) && allow_back) {
			selection = -1;
			break;
		}
		while(keydown_any(KEY_DOWN,KEY_UP, KEY_SHIFT,0)) clearevents();
	}
	return selection;
}

void draw_ui(struct Player *player, int curr_select) {
	int index = get_nb_moves(player);

	for(int i = 0; i < index; i++) {
		draw_special_move(2+132*i,DHEIGHT-65, player->moves[i], i == curr_select);
	}
}

void reset_pp(struct Player *player) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		player->moves[i]->pp = player->moves[i]->init_pp;
	}
	draw_text(50, DHEIGHT-47, C_BLACK, "Vous regagnez les PPs de l'ensemble de vos capacités");
	dupdate();
	wait_for_input(KEY_SHIFT);
}

void reset_hp(struct Player *player) {
	player->stats.pv = player->stats.max_pv;
	draw_text(50, DHEIGHT-47, C_BLACK, "Vous regagnez l'ensemble de vos PVs");
	dupdate();
	wait_for_input(KEY_SHIFT);
}

void add_xp(struct Player *player, int xp) {
	player->stats.xp += xp;

	//niveau suivant une progession N³
	int calc_level = (int)pow(player->stats.xp, 0.33);
	for(int i = player->stats.level; i < calc_level; i++) {
		player->stats.pv = player->stats.pv + player->stats.max_pv * 0.2;
		draw_text(50, DHEIGHT-47, C_BLACK, "Vous passez au niveau %d !", i+1);
		dupdate();
		wait_for_input(KEY_SHIFT);
	}
	int prec = player->stats.level;
	player->stats.level = calc_level;
	check_level(player, prec);
}

bool has_pp_left(struct Player *player) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		if(player->moves[i]->pp < player->moves[i]->init_pp) return true;
	}
	return false;
}

bool add_pp(struct Player *player, int amount) {
	if(has_pp_left(player)) {
		int selection = select_capacity(player, "Choisir une capacité", false);
		player->moves[selection]->pp += amount;
		if(player->moves[selection]->pp > player->moves[selection]->init_pp) player->moves[selection]->pp = player->moves[selection]->init_pp;
		draw_text(50, DHEIGHT-47, C_BLACK, "Vous regagnez %d PPs sur %s", amount, player->moves[selection]->name);
		dupdate();
		wait_for_input(KEY_SHIFT);
		return true;
	}
	return false;
}

void change_type(struct Player *player, struct Type type) {
	if(strcmp(player->stats.type, type.name) != 0) {
		int selection = yes_no_question("Voulez vous changer votre type %s en %s ?", player->stats.type, type.name);
		if(selection) player->stats.type = type.name;
	} else {
		draw_text(50, DHEIGHT-47, C_BLACK, "Vous êtes déjà du type %s !", type.name);
		dupdate();
		wait_for_input(KEY_SHIFT);
	}
}

int get_nb_eventzone(struct Player *player) {
	for(int i = 0; i < NB_STO_ZONE; i++) {
		if(player->eventListZone[i] == 0) {
			return i;
		}
	}
	return 0;
}

int get_nb_eventdialog(struct Player *player) {
	for(int i = 0; i < NB_STO_DIALOG; i++) {
		if(player->eventListDialog[i] == 0) {
			return i;
		}
	}
	return 0;
}

bool check_eventzone(struct Player *player, int id) {
	for(int i = 0; i < NB_STO_ZONE; i++) {
		if(player->eventListZone[i] == id) return true;
		if(player->eventListZone[i] == 0) return false;
	}
	return false;
}

bool check_eventdialog(struct Player *player, int id) {
	for(int i = 0; i < NB_STO_DIALOG; i++) {
		if(player->eventListDialog[i] == id) return true;
		if(player->eventListDialog[i] == 0) return false;
	}
	return false;
}

void player_step_back(struct Player *player) {
	int direction = player->direction;
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);
	player->pos.x = player->pos.x - dx;
	player->pos.y = player->pos.y - dy;
	player->idle = !anim_player_idle(&player->anim, 1);
}