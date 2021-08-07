#pragma once

#include "animation.h"
#include "engine.h"

struct player {
	/*current position of the player on the map*/
	int x, y;
	/*the direction the player facing to*/
	int direction;
	/*current frame of the animation*/
	int frame;
	/*where to draw the player on the screen*/
	int show_x, show_y;
	/*the current animation*/
	int idle;
	struct anim_data anim;
};

/*return the info tile value the player is facing to*/
int player_facing(struct game const *game);