#include <gint/std/stdlib.h>

#include "map.h"
#include "player.h"
#include "define.h"
#include "util.h"

extern struct Map map_1;

struct Map *maps[] = {
	&map_1,
};

/*check if a tile is walkable*/
int map_walkable(struct Map const *map, int x, int y) {
	int tile = map->info_map[x + map->w * y];
	if(x < 0 || x > map->w-1 || y < 0 || y > map->h-1) return 0;
	return (tile != TILE_SOLID && tile != TILE_TALKABLE);
}

/*get the tile under the player*/
int map_get_player_tile(struct Game const *game) {
	return game->map->info_map[game->player->pos.x + game->map->w * game->player->pos.y];
}

/*generate the interior*/
void generate_interior_map(struct Game *game) {
	const int NB_INTERIORS = 3;

	extern struct Map in_1;
	extern struct Map in_2;
	extern struct Map in_3;

	struct Map *interiors[] = {
		&in_1,
		&in_2,
		&in_3
	};

	srand(game->player->pos.x * game->player->pos.y);

	game->map = interiors[rand_range(0,NB_INTERIORS)];
	game->player->pos = locate_tile(game->map, TILE_DOOR_OUT);
	engine_center_camera(game);
}

/*may be useful later*/
void set_map(struct Game *game, int id) {
	game->map = maps[id];
	game->camera.pos = vec2f_lerp(game->camera.pos, *game->camera.target, 1);
}

/*locate the first occurence of a tile in a specified map*/
struct Vec2 locate_tile(struct Map const *map, int tile) {
	for(int x = 0; x < map->w; x++) {
		for(int y = 0; y < map->h; y++) {
			int tile_id = map->info_map[x + y * map->w];
			if(tile_id == tile) {
				return VEC2(x,y);
			}
		}
	}
	return VEC2(0,0);
}