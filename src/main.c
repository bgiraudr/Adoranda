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
extern struct character character_Lephenixnoir;
extern struct character character_Tituya2;
extern struct character character_KikooDX;
extern struct character character_Massena;
extern struct character character_PancarteVille;


struct character character_default = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.dialog = "default dialog"
};

struct character *characters[] = {
	&character_Tituya,
	&character_Lephenixnoir,
	&character_Massena,
	&character_Tituya2,
	&character_KikooDX,
	&character_PancarteVille,
	&character_default,
};

static int callback_tick(volatile int *tick) {
    *tick = 1;
    return TIMER_CONTINUE;
}

int main(void) {
	struct player player = {
		.x = 32,
		.y = 30,
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
		.characters = characters,
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
		dprint(1,20,C_BLACK, "%d", player_facing(&game));
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