#include <gint/display.h>
#include <gint/keyboard.h>
#include <math.h>

#include "stats.h"
#include "util.h"

void draw_stats(struct Stats stats) {
	drect(DWIDTH-110,0,DWIDTH,DHEIGHT,C_WHITE);
	dprint(300,20,C_BLACK,"PV : %d",stats.pv);
	dprint(300,40,C_BLACK,"Level : %d",stats.level);
	dprint(300,60,C_BLACK,"XP : %d",stats.xp);
	dprint(300,80,C_BLACK,"ATK : %d",stats.atk);
	dprint(300,100,C_BLACK,"DEF : %d",stats.def);
	dprint(300,120,C_BLACK,"LVLUP : %d",(int)(pow(stats.level+1, 3.03))-stats.xp);
}

void set_stats_level_from(const struct Stats *from, struct Stats *to) {
	to->max_pv = calc_stats(from->pv, to->level);
	to->atk = calc_stats(from->atk, to->level);
	to->def = calc_stats(from->def, to->level);
}

void set_stats_level(struct Stats *stats) {
	stats->max_pv = calc_stats(stats->pv, stats->level);
	stats->atk = calc_stats(stats->atk, stats->level);
	stats->def = calc_stats(stats->def, stats->level);
}

int calc_stats(int base, int level) {
	return (int)(((2*base + level/4 + 100) * level)/100+10);
}