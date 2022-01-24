#include "capacite.h"

#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"

extern struct Capacites capacites;

struct Move default_move() {
	return *capacites.moves[0];
}

struct Move get_move_id(int id) {
	return *capacites.moves[id];
}