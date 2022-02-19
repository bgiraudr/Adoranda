#pragma once
#include "define.h"
#include "stats.h"
#include "capacite.h"
#include "game.h"
#include <gint/display.h>

struct Monster {
	char *name;
	bopti_image_t *sprite;
	int id;
	int nbMoves;
	struct Stats *stats;
	short *moves;
};

struct Monsters {
	int nbMonsters;
	struct Monster *monster[];
};

struct Monster *get_monster_id(int id);
struct Monster *generate_monster(struct Game *game);
struct Move *monster_select(struct Player *player, struct Monster *monster);
struct Monster *copyMonster(struct Monster *source);
void free_monster(struct Monster *monster);