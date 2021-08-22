#include "map.h"
#include "engine.h"
#include "player.h"
#include "camera.h"
#include "define.h"

/*check if a tile is walkable*/
int map_walkable(struct Map const *map, int x, int y) {
	int tile = map->info_map[x + map->w * y];
	if(x < 0 || x > map->w-1 || y < 0 || y > map->h-1) return 0;
	return (tile != TILE_SOLID && tile != TILE_CHARACTER);
	return 1;
}

/*get the tile under the player*/
int map_get_player_tile(struct Game const *game) {
	return game->map->info_map[game->player->x + game->map->w * game->player->y];
	return 0;
}

void generate_interior_map(struct Game *game) {
	extern struct Map in_1;
	game->map = &in_1;
	set_player_xy(game->player, 3,3);
	game->camera->x = in_1.w/2 * TILE_SIZE + game->player->x_mid;
	game->camera->y = in_1.h/2 * TILE_SIZE + game->player->y_mid;
}

/*return true if the map is larger than the screen. false otherwise*/
bool is_map_larger(struct Map *map) {
	if(map->w > DWIDTH / TILE_SIZE + 1 && map->h > DHEIGHT / TILE_SIZE + 1) {
		return true;
	} else {
		return false;
	}
}