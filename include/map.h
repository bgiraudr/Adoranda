#pragma once
#include <gint/display.h>
#include <stdbool.h>
#include "engine.h"

struct Map {
	/*width, height and the number of layer of the map (max 2)*/
	int w, h, nb_layers, dialog_count;
	/*the tileset to use*/
	bopti_image_t *tileset;
	/*list of all the dialog*/
	struct Talkable *dialogs;
	/*state of each tile on the map (solid, air ...)*/
	short *info_map;
	/*list of all the tiles*/
	short *layers[];
};

enum map_state {
	TILE_AIR = 0,
	TILE_SOLID = 1,
	TILE_DOOR_IN = 2,
	TILE_DOOR_OUT = 3,
	TILE_TALKABLE = 4,
};

/*check if a tile is walkable*/
int map_walkable(struct Map const *map, int x, int y);
/*get the tile under the player*/
int map_get_player_tile(struct Game const *game);

void generate_interior_map(struct Game *game);