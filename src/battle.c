#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"
#include "monster.h"

void create_battle(struct Game *game) {
	game->player->stats.pv = game->player->stats.max_pv;
	during_battle(game->player, generate_monster(game));
}

int during_battle(struct Player *player, struct Monster monster) {
	int tour = 0;
	int selection = 0;
	while(1) {
		dclear(C_RGB(25,25,25));
		draw_battle(player, &monster);
		dupdate();
		selection = select_move(player, &monster, selection);
		draw_executed_move(player->moves[selection], &monster, 0);
		wait_for_input(KEY_SHIFT);
		execute_move(&player->stats, monster.stats, player->moves[selection]);
		if(player->stats.pv <= 0) {
			return LOSE;
		}
		if(monster.stats->pv <= 0) {
			return WIN;
		}
		tour++;
	}
	return LOSE;
}

int select_move(struct Player *player, struct Monster *monster, int prec_selected) {
	const int nbMove = get_nb_moves(player);
	int selection = prec_selected;
	int buffer = keydown(KEY_SHIFT);
	
	while(1) {
		clearevents();

		selection += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
		
		if(selection > nbMove-1) selection = nbMove-1;
		if(selection < 0) selection = 0;

		dclear(C_RGB(25,25,25));
		draw_battle(player, monster);
		draw_ui(player);
		dtext(58 + (selection * 130), DHEIGHT-15 , C_RED, "[X]");
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		if(keydown(KEY_EXIT)) {
			player->stats.pv--;
			break;
		}
		while(keydown_any(KEY_RIGHT, KEY_LEFT, KEY_SHIFT, 0)) clearevents();
	}
	return selection;
}

void draw_battle(struct Player *player, struct Monster *monster) {
	const int WIDTH_HP = 100;
	int posHP = (float)player->stats.pv / player->stats.max_pv * WIDTH_HP;
	drect(10,15,10+WIDTH_HP,25,C_BLACK);
	drect(10,15,10+posHP,25,C_GREEN);

	dprint(15+WIDTH_HP,15,C_BLACK,"%d/%d", player->stats.pv, player->stats.max_pv);

	int posHPmonster = (float)monster->stats->pv / monster->stats->max_pv * WIDTH_HP;
	dtext(240,2,C_BLACK,monster->name);
	drect(240,15,240+WIDTH_HP,25,C_BLACK);
	drect(240,15,240+posHPmonster,25,C_GREEN);

	dprint(245+WIDTH_HP,15,C_BLACK,"%d/%d", monster->stats->pv, monster->stats->max_pv);
	dimage(260,30,monster->sprite);
}

void draw_executed_move(struct Move move, struct Monster *monster, int is_monster) {
	const int rect_size = 100;
	drect(0,DHEIGHT,DWIDTH,DHEIGHT-rect_size,C_WHITE);
	if(is_monster) {
		dprint(10,DHEIGHT-rect_size/2-8, C_BLACK, "%s lance %s !", monster->name, move.name);
	} else {
		dprint(10,DHEIGHT-rect_size/2-8, C_BLACK, "Vous lancez %s !", move.name);
	}
	dupdate();
}