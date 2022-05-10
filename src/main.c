#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>

#include <gint/usb-ff-bulk.h>

#include "game.h"
#include "engine.h"
#include "player.h"

static int callback_tick(volatile int *tick) {
	*tick = 1;
	return TIMER_CONTINUE;
}

void take_capture(void) {
	if (keydown(KEY_VARS) && usb_is_open()) {
		usb_fxlink_screenshot(1);
	}
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

	usb_interface_t const *interfaces[] = {&usb_ff_bulk, NULL};
	usb_open(interfaces, GINT_CALL_NULL);

	/*Font*/
	extern font_t uf8x9;
	dfont(&uf8x9);

	dupdate_set_hook(GINT_CALL(take_capture));

	/*Main loop*/
	while(1) {
		while(!tick) sleep();
		tick = 0;

		engine_draw(&game);
		dupdate();

		int action = 0;
		while(action != -1) {
			action = get_inputs();
			if(action == ACTION_MENU) break;
			if(action >= DIR_DOWN && action <= DIR_RIGHT)
				engine_move(&game, action);
			else if(action >= ACTION_SHIFT) {
				engine_action(&game, action);
			}
		}
		if(action == ACTION_MENU) break;
		engine_tick(&game, ENGINE_TICK);
	}

	if(t >= 0) timer_stop(t);
	usb_close();
	return 0;
}
