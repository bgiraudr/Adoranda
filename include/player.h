#pragma once

#include "animation.h"
#include "engine.h"

struct Player {
	/*current position of the player on the map - Tile*/
	int x, y;
	/*player mid - offset pixels*/
	int x_mid, y_mid;
	/*the direction the player facing to*/
	int direction;
	/*current frame of the animation*/
	int frame;
	/*where to draw the player on the screen*/
	int show_x, show_y;
	/*the current animation*/
	int idle;
	struct AnimData anim;
};

/*return the info tile value the player is facing to*/
int player_facing(struct Game const *game);
