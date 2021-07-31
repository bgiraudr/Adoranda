#pragma once

#include "animation.h"

struct player {
	int x, y;
	int direction;
	int frame;
	int idle;
	struct anim_data anim;
};

enum direction {
	DIR_DOWN = 0,
	DIR_LEFT = 1,
	DIR_UP = 2,
	DIR_RIGHT = 3
};