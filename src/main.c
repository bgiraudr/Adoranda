#include <gint/display.h>
#include <gint/keyboard.h>
#include "game.h"
#include "map.h"
#include "engine.h"
#include "player.h"
#include "animation.h"

#include <gint/timer.h>
#include <gint/clock.h>

extern struct map map_1;

struct map *maps[] = {
	&map_1,
};

static int callback_tick(volatile int *tick) {
    *tick = 1;
    return TIMER_CONTINUE;
}

int main(void) {
	struct player player = {
		.x = 16,
		.y = 7,
		.show_x = 12,
		.show_y = 7,
		.direction = DIR_DOWN,
		.anim.function = anim_player_idle,
		.anim.dir = DIR_DOWN
	};
	player.idle = !anim_player_idle(&player.anim, 1);

	struct game game = {
		.map = maps[0],
		.player = &player
	};

	static volatile int tick = 1;

	int t = timer_configure(TIMER_ANY, ENGINE_TICK*1000,
		GINT_CALL(callback_tick, &tick));
	if(t >= 0) timer_start(t);

	while(!keydown(KEY_7)) {
		while(!tick) sleep();
		tick = 0;

		engine_draw(&game);
		dupdate();

		int dir = get_inputs();
		if(dir >= 0) 
			engine_move(&game, dir);
		engine_tick(&game, ENGINE_TICK);
	}

	if(t >= 0) timer_stop(t);
	return 0;
}