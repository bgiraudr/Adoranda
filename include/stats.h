#pragma once

struct Stats {
	int atk;
	int def;
	int pv;
	int level;
	int xp;
	int max_pv;
};

void draw_stats(struct Stats stats);