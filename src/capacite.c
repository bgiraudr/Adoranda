#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>

#include "capacite.h"

extern struct Capacities capacities;

struct Move default_move() {
	return *capacities.moves[0];
}

struct Move get_move_id(int id) {
	return *capacities.moves[id];
}

void draw_move(int x, int y, int x2, int y2, struct Move move) {
	const int font_size = 8;
	drect(x, y, x2, y2, C_WHITE);
	dprint(x+5, y+5, C_BLACK, "PP : %d", move.pp);
	dprint(x+5, y2-15, C_BLACK, "ATK : %d", move.atk);
	dprint((int)((x+x2)/2)-(int)(strlen(move.name)/2*font_size), 
		(int)((y+y2)/2)-font_size/2, 
		C_BLACK, "%s", move.name);
}

void draw_classic_move(int x, int y, struct Move move) {
	draw_move(x, y, x+170, y+60, move);
}