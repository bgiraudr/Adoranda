#pragma once

struct map {
	int w, h, nb_layer;
	bopti_image_t *tileset;
	short *tiles_layer1;
	short *tiles_layer2;
	short *info_map;
};

enum map_state {
	TILE_AIR = 0,
	TILE_SOLID = 1,
	TILE_DOOR = 2,
	TILE_CHARACTER = 3,
};
