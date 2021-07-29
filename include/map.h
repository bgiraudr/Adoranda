#pragma once

struct map {
	int w, h;
	bopti_image_t *tileset;
	short *tiles;
};

enum map_state {
	TILE_AIR = 0,
	TILE_SOLID = 1,
	TILE_WALKABLE = 2
};
