#pragma once

struct map {
	int w, h;
	bopti_image_t *tileset;
	short *tiles_layer1;
	short *tiles_layer2;
};

enum map_state {
	TILE_AIR = 0,
	TILE_SOLID = 1,
	TILE_WALKABLE = 2
};
