#include <gint/display.h>
#include <gint/keyboard.h>
#include "game.h"
#include "map.h"
#include "engine.h"
#include "player.h"
#include "animation.h"
#include "camera.h"
#include "define.h"

#include <gint/timer.h>
#include <gint/clock.h>

extern struct Map map_1;

struct Map *maps[] = {
	&map_1,
};

static int callback_tick(volatile int *tick) {
	*tick = 1;
	return TIMER_CONTINUE;
}

int main(void) {
	/*Structure definition*/
	struct Player player = {
		.pos = VEC2(32, 30),
		.pos_visual = VEC2F(32*TILE_SIZE, 30*TILE_SIZE),
		.x_mid = 6,
		.y_mid = 1,
		.show_x = 12,
		.show_y = 7,
		.direction = DIR_DOWN,
		.anim.function = anim_player_idle,
		.anim.dir = DIR_DOWN
	};
	player.idle = !anim_player_idle(&player.anim, 1);

	struct Game game = {
		.map = maps[0],
		.player = &player,
		.camera = camera_new(&player.pos_visual),
		.background = C_WHITE
	};

	/*Timer*/
	static volatile int tick = 1;

	int t = timer_configure(TIMER_ANY, ENGINE_TICK*1000,
		GINT_CALL(callback_tick, &tick));
	if(t >= 0) timer_start(t);

	/*Main loop*/
	while(!keydown(KEY_MENU)) {
		while(!tick) sleep();
		tick = 0;

		engine_draw(&game);
		dupdate();

		int action = get_inputs();
		if(action >= 0 && action <= 3)
			engine_move(&game, action);
		else if(action >= 4) {
			engine_action(&game, action);
		}
		engine_tick(&game, ENGINE_TICK);
	}

	if(t >= 0) timer_stop(t);
	return 0;
}
