#pragma once
#include "player.h"
#include "monster.h"

enum battle_state {
	EXIT = 0,
	LOSE = 1,
	WIN = 2,
};

void create_battle(struct Game *game);
int during_battle(struct Player *player, struct Monster *monster);
int select_move(struct Player *player, struct Monster *monster, int prec_selected);
void draw_battle(struct Player *player, struct Monster *monster);
void draw_executed_move(struct Move move, struct Monster *monster, int is_monster);