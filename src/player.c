#include <gint/keyboard.h>
#include <stdlib.h>

#include "player.h"
#include "define.h"
#include "map.h"
#include "stats.h"
#include "capacite.h"
#include "util.h"

struct Player init_player(void) {

	struct Stats bstats = {
		.atk = 15,
		.def = 15,
		.level = 1,
		.pv = 30,
	};

	struct Stats stats = {
		.level = 1,
		.xp = 0
	};

	struct Player player = {
		.pos = VEC2(32, 30),
		.pos_visual = VEC2F(32*TILE_SIZE, 30*TILE_SIZE),
		.base_stats = bstats,
		.stats = stats,
		.x_mid = 6,
		.y_mid = 1,
		.show_x = 12,
		.show_y = 7,
		.direction = DIR_DOWN,
		.anim.function = anim_player_idle,
		.anim.dir = DIR_DOWN
	};
	player.idle = !anim_player_idle(&player.anim, 1);
	player.moves[0] = copy_move(default_move());

	set_stats_level_from(&player.base_stats, &player.stats);
	return player;
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

void add_move(struct Player *player, struct Move move) {
	int index = get_nb_moves(player);
	if(index != NB_PLAYER_MOVES) player->moves[index] = copy_move(move);
	else replace_capacities(player, move);
}

void draw_player_moves(struct Player *player) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		draw_classic_move(0,65*i,player->moves[i]);
	}
}

void replace_capacities(struct Player *player, struct Move move) {
	int selection = 0;
	int buffer = keydown(KEY_SHIFT);
	while(1) {
		clearevents();

		selection += keydown(KEY_DOWN) - keydown(KEY_UP);
		if(selection > NB_PLAYER_MOVES-1) selection = NB_PLAYER_MOVES-1;
		if(selection < 0) selection = 0;

		// draw_classic_move(200,DHEIGHT/2-30, move);
		for(int i = 0; i < NB_PLAYER_MOVES; i++) {
			draw_classic_move(0,65*i, player->moves[i]);
		}
		
		dtext(105, 45+65*selection , C_RED, "[X]");
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

void draw_ui(struct Player *player) {
	int index = get_nb_moves(player);

	for(int i = 0; i < index; i++) {
		draw_classic_move(2+132*i,DHEIGHT-70, player->moves[i]);
	}
}

void reset_pp(struct Player *player) {
	int index = get_nb_moves(player);
	for(int i = 0; i < index; i++) {
		player->moves[i]->pp = player->moves[i]->init_pp;
	}
}