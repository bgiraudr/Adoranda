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

void engine_draw(struct game const *game);
void engine_draw_map_around_player(struct game const *game);
void engine_draw_player(struct player const *player);
int engine_move(struct game *game, int direction);
void engine_tick(struct game *game, int dt);
void engine_set_background(struct game *game, int color);
void engine_action(struct game const *game, int action);
void engine_check_position(struct game *game);