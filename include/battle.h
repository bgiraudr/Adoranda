#pragma once
#include "player.h"

void create_battle(struct Player *player);
int during_battle(struct Player *player);

enum battle_state {
	EXIT = 0,
	LOSE = 1,
	WIN = 2,
};