#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>

#include "game.h"
#include "engine.h"
#include "player.h"

static int callback_tick(volatile int *tick) {
	*tick = 1;
	return TIMER_CONTINUE;
}

int main(void) {
	/*Structure definition*/
	struct Player player = init_player();
	struct Game game = init_game(&player);

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
