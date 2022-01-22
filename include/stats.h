#pragma once

struct Stats {
	int atk;
	int def;
	int pv;
	int level;
	int xp;
};

void draw_stats(struct Stats stats);