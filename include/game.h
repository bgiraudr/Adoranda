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

enum action {
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	ACTION_SHIFT,
	ACTION_MENU,
	ACTION_ALPHA,
	ACTION_OPTN,
	ACTION_F1,
	ACTION_F2,
};

/*get the input with a timeout*/
int get_inputs(void);
struct Game init_game(struct Player *player);