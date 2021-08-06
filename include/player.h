#pragma once

#include "animation.h"
#include "engine.h"

struct player {
	int x, y;
	int direction;
	int frame;
	int show_x, show_y;
	int idle;
	struct anim_data anim;
};

int player_facing(struct game const *game);