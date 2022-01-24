#include "player.h"
#include "define.h"
#include "map.h"
#include "stats.h"
#include "capacite.h"

struct Player init_player(void) {

	struct Stats stats = {
		.atk = 1,
		.def = 1,
		.level = 1,
		.pv = 10,
		.xp = 0
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
		//TODO remplacer une capacité
	}
}