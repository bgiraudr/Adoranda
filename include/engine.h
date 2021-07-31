#pragma once

#define ENGINE_TICK 25

struct game {
	struct map *map;
	struct player *player;
};

void engine_draw(struct game const *game);
void engine_draw_player(struct player const *player);
void engine_move(struct game *game, int direction);
int map_walkable(struct map const *map, int x, int y);