#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include <math.h>

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
	draw_move(x, y, x+125, y+60, move);
}

void execute_move(struct Stats *player_stats, struct Stats *monster_stats, struct Move move, int ismonster) {
	if(ismonster) {
		player_stats->pv-=calc_damage(monster_stats, player_stats, move);
	} else {
		monster_stats->pv-=calc_damage(player_stats, monster_stats, move);
	}
}

int calc_damage(struct Stats *attacker, struct Stats *target, struct Move move) {
	return(floor(((2*attacker->level / 5 + 2) * attacker->atk * move.atk / target->def) / 50) + 2);
}