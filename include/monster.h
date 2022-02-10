#pragma once
#include "define.h"
#include "stats.h"
#include "capacite.h"
#include "game.h"
#include <gint/display.h>

struct Monster {
	char *name;
	bopti_image_t *sprite;
	struct Stats *stats;
	short *moves;
};

struct Monster generate_monster(struct Game *game);