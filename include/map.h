#pragma once
#include <gint/display.h>
#include "engine.h"

struct Teleporter {
	int x, y;
	int idMap;
	int x2, y2;
};

struct Zone {
	int start_x, start_y;
	int end_x, end_y;
	char *event;
	int level;
	int nbMonsters;
	short *monsters;
};

struct Map {
	/*width, height and the number of layer of the map*/
	int w, h, nb_layers, dialog_count, teleporter_count, zone_count;
	/*the tileset to use*/
	bopti_image_t *tileset;
	int tileset_size;
	/*list of all the dialog*/
	struct Talkable *dialogs;
	struct Teleporter *teleporters;
	struct Zone *zones;
	/*state of each tile on the map (solid, air ...)*/
	short *info_map;
	/*list of all the tiles*/
	short *layers[];
};

enum map_state {
	TILE_AIR,
	TILE_SOLID,
	TILE_DOOR_IN,
	TILE_DOOR_OUT,
	TILE_TALKABLE,
	TILE_TELEPORTER,
	TILE_GRASS,
	TILE_ICE,
};

/*check if a tile is walkable*/
int map_walkable(struct Map const *map, int x, int y);
/*get the tile under the player*/
int map_get_player_tile(struct Game const *game);

void generate_interior_map(struct Game *game);

void set_map(struct Game *game, int id);

struct Vec2 locate_tile(struct Map const *map, int tile);

struct Teleporter get_teleporter_xy(struct Map *map, struct Vec2 pos);

int is_in_zone(struct Player *player, struct Map *map);
struct Zone get_zone(struct Player *player, struct Map *map);