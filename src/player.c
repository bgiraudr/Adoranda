#include "player.h"
#include "engine.h"
#include "map.h"
#include "game.h"

/*
return the info tile value the player is facing to
TILE_SOLID by default (out of bound)
*/
int player_facing(struct Game const *game) {
	int direction = game->player->direction;
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	int index = game->player->x + dx + game->map->w * (game->player->y + dy);
	if(game->player->x + dx >= 0 &&
		game->player->x + dx <= game->map->w &&
		game->player->y + dy >= 0 &&
		game->player->y + dy <= game->map->h) {
		return game->map->info_map[index];
	}
	return TILE_SOLID;
}

void set_player_xy(struct Player *player, int x, int y) {
	player->x = x;
	player->y = y;
}