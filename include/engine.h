#pragma once

struct game {
	struct map *map;
	struct player *player;
};

void engine_draw(struct game const *game);
void engine_draw_player(struct player const *player);
void engine_move(struct player *player, int direction);