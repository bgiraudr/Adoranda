#pragma once

struct Move {
	char *name;
	int pp;
	int atk;
};

struct Capacites {
	int nbCapacites;
	struct Move *moves[];
};

struct Move default_move();
struct Move get_move_id(int id);