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

static int callback_tick(volatile int *tick)
{
    *tick = 1;
    return TIMER_CONTINUE;
}

void play() {
	struct player player = {
		.x = 16,
		.y = 7,
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
		//engine_draw_player(game->player);
		dupdate();

		int dir = get_inputs();
		if(dir >= 0) 
			engine_move(&game, dir);
		engine_tick(&game, ENGINE_TICK);
	}
	if(t >= 0) timer_stop(t);
}

int get_inputs(void) {
	int opt = GETKEY_DEFAULT & ~GETKEY_REP_ARROWS;
	int timeout = 1;

    while(1) {

    	key_event_t ev = getkey_opt(opt, &timeout);
		if(ev.type == KEYEV_NONE) return -1;
        int key = ev.key;

        if(key == KEY_DOWN)  return DIR_DOWN;
        if(key == KEY_RIGHT) return DIR_RIGHT;
        if(key == KEY_UP)    return DIR_UP;
        if(key == KEY_LEFT)  return DIR_LEFT;
    }
}