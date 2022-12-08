#pragma once
#include "player.h"
#include "monster.h"

enum battle_state {
	EXIT,
	LOSE,
	WIN,
};

void create_battle(struct Game *game);
int battle(struct Player *player, struct Monster *monster);
int select_move(struct Player *player, struct Monster *monster, int prec_selected);
void draw_battle(struct Player *player, struct Monster *monster);
void draw_executed_move(struct Move *move, struct Monster *monster, int is_monster);
void draw_status(char *name, char *message);
void finish_battle(int status, struct Game *game, struct Monster *monster);
void check_move_status(int status, struct Player *player, struct Monster *monster, struct Move *move, int is_monster);