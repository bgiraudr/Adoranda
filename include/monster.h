#pragma once
#include "define.h"
#include "stats.h"
#include "capacite.h"
#include "game.h"
#include <gint/display.h>

struct Monster {
	char *name;
	struct Stats stats;
	struct Move moves[NB_PLAYER_MOVES];
	bopti_image_t *sprite;
};

struct Monster generate_monster(struct Game *game);