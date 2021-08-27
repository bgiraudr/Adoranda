#pragma once
#include "camera.h"

struct Game {
	/*the current map to display*/
	struct Map *map;
	/*the player*/
	struct Player *player;
	/*the camera*/
	struct Camera camera;
	/*the background color*/
	int background;
};

enum direction {
	DIR_DOWN = 0,
	DIR_LEFT = 1,
	DIR_UP = 2,
	DIR_RIGHT = 3,
	ACTION_SHIFT = 4,
	ACTION_ALPHA = 5
};

/*get the input with a timeout*/
int get_inputs(void);
struct Game init_game(struct Player *player);