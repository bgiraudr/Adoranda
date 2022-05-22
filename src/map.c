#include <gint/std/stdlib.h>
#include <gint/keyboard.h>
#include "map.h"
#include "player.h"
#include "define.h"
#include "util.h"

extern struct Map map_1;
extern struct Map map_2;

struct Map *maps[] = {
	&map_1,
	&map_2,
};

struct Zone default_zone = {
	.start_x = 0,
	.start_y = 0,
	.end_x = 0,
	.end_y = 0,
	.event = "",
	.level = -1,
	.nbMonsters = 0
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
	extern struct Map in_1;
	extern struct Map in_2;

	struct Map *interiors[] = {
		&in_1,
		&in_2,
	};

	srand(game->player->pos.x * game->player->pos.y);

	game->map = interiors[rand_range(0,NB_INTERIORS)];
	game->player->pos = locate_tile(game->map, TILE_DOOR_OUT);
}

void set_map(struct Game *game, int id) {
	game->map = maps[id];
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

struct Teleporter get_teleporter_xy(struct Map *map, struct Vec2 pos) {
	int i = 0;
	while(i < map->teleporter_count) {
		if(map->teleporters[i].x == pos.x && map->teleporters[i].y == pos.y) return map->teleporters[i];
		i++;
	}
	return map->teleporters[0];
}

int is_in_zone(struct Player *player, struct Map *map) {
	const int posx = player->pos.x;
	const int posy = player->pos.y;
	for(int i = 0; i < map->zone_count; i++) {
		struct Zone zone = map->zones[i];
		if(zone.start_x <= posx && zone.start_y <= posy && zone.end_x >= posx && zone.end_y >= posy) return 1;
	}
	return 0;
}

struct Zone get_zone(struct Player *player, struct Map *map) {
	const int posx = player->pos.x;
	const int posy = player->pos.y;
	for(int i = 0; i < map->zone_count; i++) {
		struct Zone zone = map->zones[i];
		if(zone.start_x <= posx && zone.start_y <= posy && zone.end_x >= posx && zone.end_y >= posy) return zone;
	}
	return default_zone;
}

void addZoneToPlayer(struct Player *player, struct Zone zone) {
	player->eventListZone[get_nb_eventzone(player)] = zone.id;
}