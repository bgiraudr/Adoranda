#include <gint/keyboard.h>

#include "player.h"
#include "define.h"
#include "map.h"
#include "stats.h"
#include "capacite.h"
#include "util.h"

struct Player init_player(void) {

	struct Stats stats = {
		.atk = 1,
		.def = 1,
		.level = 1,
		.pv = 10,
		.xp = 0,
		.max_pv = 10,
	};

	struct Player player = {
		.pos = VEC2(32, 30),
		.pos_visual = VEC2F(32*TILE_SIZE, 30*TILE_SIZE),
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
	player.moves[0] = default_move();

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

void add_move(struct Player *player, struct Move move) {
	if(player->moves[1].name == NULL) {
		player->moves[1] = move;
	} else {
		replace_capacities(player, move);
	}
}

void draw_player_moves(struct Player *player) {
	draw_classic_move(0,0,player->moves[0]);
	if(player->moves[1].name != NULL) {
		draw_classic_move(0,80,player->moves[1]);
	}
}

void replace_capacities(struct Player *player, struct Move move) {
	int selection = 0;
	int buffer = keydown(KEY_SHIFT);
	while(1) {
		clearevents();

		selection += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
		if(selection > 1) selection = 1;
		if(selection < 0) selection = 0;

		draw_classic_move(115,DHEIGHT/2-65, move);
		draw_classic_move(20,DHEIGHT/2+5, player->moves[0]);
		draw_classic_move(210,DHEIGHT/2+5, player->moves[1]);
		
		dtext(95 + (selection * 190), DHEIGHT/2+50 , C_RED, "[X]");
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		if(keydown(KEY_EXIT)) {
			selection = -1;
			break;
		}
		while(keydown(KEY_SHIFT)) clearevents();
	}
	if(selection >= 0) {
		player->moves[selection] = move;
	}
}

void draw_ui(struct Player *player) {
	draw_classic_move(20,DHEIGHT-80, player->moves[0]);
	if(player->moves[1].name != NULL) {
		draw_classic_move(210,DHEIGHT-80, player->moves[1]);
	}

	const int WIDTH_HP = 100;
	int posHP = (float)player->stats.pv / player->stats.max_pv * WIDTH_HP;
	drect(10,10,10+WIDTH_HP,20,C_BLACK);
	drect(10,10,10+posHP,20,C_GREEN);

	dprint(15+WIDTH_HP,10,C_BLACK,"%d/%d", player->stats.pv, player->stats.max_pv);
}