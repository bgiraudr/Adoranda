#pragma once

struct Stats {
	char* type;
	int atk;
	int def;
	int pv;
	int spe_atk;
	int spe_def;
	int level;
	int xp;
	int max_pv;
};

void draw_stats(struct Stats stats);
void set_stats_level(struct Stats *stats);
int calc_stats(int base, int level);
void set_stats_level_from(const struct Stats *from, struct Stats *to);