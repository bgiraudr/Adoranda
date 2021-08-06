#include <gint/display.h>
#include <gint/keyboard.h>
#include "game.h"
#include "map.h"
#include "engine.h"
#include "player.h"
#include "animation.h"
#include "character.h"

#include <gint/timer.h>
#include <gint/clock.h>

extern struct map map_1;

struct map *maps[] = {
	&map_1,
};

extern struct character character_Tituya;

struct character character_default = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.dialog = "default dialog"
};

struct character *characters[] = {
	&character_Tituya,
	&character_default,
};

static int callback_tick(volatile int *tick) {
    *tick = 1;
    return TIMER_CONTINUE;
}

int main(void) {
	struct player player = {
		.x = 35,
		.y = 31,
		.show_x = 12,
		.show_y = 7,
		.direction = DIR_DOWN,
		.anim.function = anim_player_idle,
		.anim.dir = DIR_DOWN
	};
	player.idle = !anim_player_idle(&player.anim, 1);

	struct game game = {
		.map = maps[0],
		.player = &player,
		.background = C_WHITE
	};

	static volatile int tick = 1;

	int t = timer_configure(TIMER_ANY, ENGINE_TICK*1000,
		GINT_CALL(callback_tick, &tick));
	if(t >= 0) timer_start(t);

	while(!keydown(KEY_MENU)) {
		while(!tick) sleep();
		tick = 0;

		engine_draw(&game);
		draw_dialog(get_character_xy(characters, 39, 30));
		dupdate();

		int dir = get_inputs();
		if(dir >= 0) 
			engine_move(&game, dir);
		engine_tick(&game, ENGINE_TICK);
	}

	if(t >= 0) timer_stop(t);
	return 0;
}