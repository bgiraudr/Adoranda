#pragma once
#include "define.h"
#include "stats.h"
#include "capacite.h"
#include "game.h"
#include <gint/display.h>

struct Monster {
	const char *name;
	bopti_image_t *sprite;
	int nbMoves;
	struct Stats *stats;
	short *moves;
};

struct Monster *generate_monster(struct Game *game);
struct Move monster_select(struct Player *player, struct Monster *monster);
struct Monster *copyMonster(struct Monster *source);
void free_monster(struct Monster *monster);