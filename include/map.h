#pragma once

#include <gint/display.h>
#include "engine.h"

struct map {
	/*width, height and the number of layer of the map (max 2)*/
	int w, h, nb_layer;
	/*the tileset to use*/
	bopti_image_t *tileset;
	/*list of all background tiles (layer 1)*/
	short *tiles_layer1;
	/*list of all "physical" tiles (layer 2)*/
	short *tiles_layer2;
	/*state of each tile on the map (solid, air ...)*/
	short *info_map;
};

enum map_state {
	TILE_AIR = 0,
	TILE_SOLID = 1,
	TILE_DOOR = 2,
	TILE_CHARACTER = 3,
};

int map_walkable(struct map const *map, int x, int y);
int map_get_player_tile(struct game const *game);