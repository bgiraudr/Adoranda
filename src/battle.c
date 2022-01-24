#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"

void create_battle(struct Player *player) {
	player->stats.level++;
	add_move(player, get_move_id(1));
}