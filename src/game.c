#include <gint/display.h>
#include <gint/keyboard.h>
#include "game.h"
#include "map.h"
#include "engine.h"
#include "player.h"

extern struct map map_1;

struct map *maps[] = {
	&map_1,
};

void play() {
	struct player player = {
		.x = 2,
		.y = 3,
		.direction = 0
	};

	struct game game = {
		.map = maps[0],
		.player = &player
	};

	while(!keydown(KEY_7)) {
		engine_draw(&game);
		dupdate();

		int dir = get_inputs();
		engine_move(&player, dir);
	}
}

int get_inputs(void)
{
    while(1)
    {
        int key = getkey().key;

        if(key == KEY_DOWN)  return DIR_DOWN;
        if(key == KEY_RIGHT) return DIR_RIGHT;
        if(key == KEY_UP)    return DIR_UP;
        if(key == KEY_LEFT)  return DIR_LEFT;
    }
}