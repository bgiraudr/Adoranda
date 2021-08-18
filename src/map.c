#include "map.h"
#include "engine.h"
#include "player.h"

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
