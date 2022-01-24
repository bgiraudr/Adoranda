#pragma once

struct Move {
	char *name;
	int pp;
	int atk;
};

struct Capacities {
	int nbCapacities;
	struct Move *moves[];
};

struct Move default_move();
struct Move get_move_id(int id);
void draw_move(int x, int y, int x2, int y2, struct Move move);
void draw_classic_move(int x, int y, struct Move move);