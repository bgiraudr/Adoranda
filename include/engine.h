#pragma once

#define ENGINE_TICK 35

struct Game {
	/*the current map to display*/
	struct Map *map;
	/*the player*/
	struct Player *player;
	/*list of all the characters on the map*/
	struct Character **characters;
	/*the camera*/
	struct Camera *camera;
	/*the background color*/
	int background;
};

/*draw the current state of the game*/
void engine_draw(struct Game const *game);
/*draw the map around the player*/
void engine_draw_map(struct Game const *game);
/*draw the player*/
void engine_draw_player(struct Game const *game);
/*move the player to the direction*/
int engine_move(struct Game *game, int direction);
/*update the player animation*/
void engine_tick(struct Game *game, int dt);
/*set the background color*/
void engine_set_background(struct Game *game, int color);
/*make an interaction with something*/
void engine_action(struct Game const *game, int action);
/*check the current position of the player. To perform action depends of his location*/
void engine_check_position(struct Game *game);
void vec_lerp(struct Camera *from, struct Player const *to, float scale);