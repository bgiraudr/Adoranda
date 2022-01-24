#include <gint/display.h>
#include <gint/keyboard.h>

#include "stats.h"
#include "util.h"

void draw_stats(struct Stats stats) {
	drect(DWIDTH-110,0,DWIDTH,DHEIGHT,C_WHITE);
	dprint(300,20,C_BLACK,"PV : %d",stats.pv);
	dprint(300,40,C_BLACK,"Level : %d",stats.level);
	dprint(300,60,C_BLACK,"XP : %d",stats.xp);
	dprint(300,80,C_BLACK,"ATK : %d",stats.atk);
	dprint(300,100,C_BLACK,"DEF : %d",stats.def);
}