#pragma once
#include "animation.h"
#include "engine.h"
#include "vec2.h"

struct Player {
	/*current position of the player on the map - Tile*/
	struct Vec2 pos;
	/*current position of the player on the map - pixels */
	struct Vec2f pos_visual;
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
void set_player_xy(struct Player *player, int x, int y);
