#pragma once

#define ENGINE_TICK 35

struct game {
	/*the current map to display*/
	struct map *map;
	/*the player*/
	struct player *player;
	/*list of all the characters on the map*/
	struct character **characters;
	/*the background color*/
	int background;
};

/*draw the current state of the game*/
void engine_draw(struct game const *game);
/*draw the map around the player*/
void engine_draw_map_around_player(struct game const *game);
/*draw the player*/
void engine_draw_player(struct player const *player);
/*move the player to the direction*/
int engine_move(struct game *game, int direction);
/*update the player animation*/
void engine_tick(struct game *game, int dt);
/*set the background color*/
void engine_set_background(struct game *game, int color);
/*make an interaction with something*/
void engine_action(struct game const *game, int action);
/*check the current position of the player. To perform action depends of his location*/
void engine_check_position(struct game *game);