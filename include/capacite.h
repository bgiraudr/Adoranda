#pragma once
#include "stats.h"

struct Move {
	char *name;
	int id;
	int categorie;
	int init_pp;
	int pp;
	int atk;
	int precision;

	//self effect (%)
	int boost_atk;
	int boost_def;
	int boost_hp;
};

struct Capacities {
	int nbCapacities;
	struct Move *moves[];
};

enum status {
	MISS,
	SUCCESS,
	CRIT,
	HEAL,
	ATK,
	DEF,
	MULTIPLE
};

enum categorie {
	STATUT,
	PHYSICAL,
};

struct Move default_move();
struct Move get_move_id(int id);
void draw_move(int x, int y, int x2, int y2, struct Move *move);
void draw_classic_move(int x, int y, struct Move *move);
int execute_move(struct Stats *player_stats, struct Stats *monster_stats, struct Move *move, int ismonster);
int calc_damage(struct Stats *attacker, struct Stats *target, struct Move *move);
struct Move *copy_move(struct Move move);
struct Move *get_move_id_pointer(int id);
float crit(struct Stats *attacker);
int is_crit();
int is_miss(struct Move *move);
int self_effect(struct Stats *stats, struct Move *move);