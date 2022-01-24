#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"

void create_battle(struct Player *player) {
	player->stats.level++;
	drect(DWIDTH-110,0,DWIDTH,DHEIGHT,C_WHITE);
	dprint(300,20,C_BLACK,"%s",player->moves[0].name);
	dprint(300,40,C_BLACK,"%s",player->moves[1].name);
	dupdate();
	wait_for_input(KEY_SHIFT);
	add_move(player, get_move_id(1));
}