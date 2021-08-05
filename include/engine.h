#pragma once

#define ENGINE_TICK 35

struct game {
	struct map *map;
	struct player *player;
	int background;
};

void engine_draw(struct game const *game);
void engine_draw_map_around_player(struct game const *game);
void engine_draw_player(struct player const *player);
int engine_move(struct game *game, int direction);
int map_walkable(struct map const *map, int x, int y);
void engine_tick(struct game *game, int dt);
void engine_set_background(struct game *game, int color);