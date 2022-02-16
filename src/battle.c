#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"
#include "monster.h"
#include <stdlib.h>
#include <math.h>
#include <gint/rtc.h>

extern bopti_image_t img_dialogue;

/**
 * create a new battle
 */
void create_battle(struct Game *game) {
	game->player->stats.pv = game->player->stats.max_pv;
	struct Monster *monster = generate_monster(game);
	int status = battle(game->player, monster);

	finish_battle(status, game, monster);
}

/**
 * Launch the battle
 */
int battle(struct Player *player, struct Monster *monster) {
	int tour = 0;
	int selection = 0;
	while(1) {
		draw_battle(player, monster);
		dupdate();
		selection = select_move(player, monster, selection);
		draw_executed_move(player->moves[selection], monster, 0);
		dupdate();
		wait_for_input(KEY_SHIFT);
		execute_move(&player->stats, monster->stats, player->moves[selection], 0);

		if(is_crit()) {
			draw_battle(player, monster);
			draw_crit();
			dupdate();
			wait_for_input(KEY_SHIFT);
		}

		draw_battle(player, monster);

		if(player->stats.pv <= 0) {
			return LOSE;
		}
		if(monster->stats->pv <= 0) {
			return WIN;
		}

		struct Move *monster_move = monster_select(player, monster);
		draw_executed_move(monster_move, monster, 1);
		dupdate();
		wait_for_input(KEY_SHIFT);
		execute_move(&player->stats, monster->stats, monster_move, 1);

		if(is_crit()) {
			draw_battle(player, monster);
			draw_crit();
			dupdate();
			wait_for_input(KEY_SHIFT);
		}

		if(player->stats.pv <= 0) {
			return LOSE;
		}
		if(monster->stats->pv <= 0) {
			return WIN;
		}
		tour++;
	}
	return LOSE;
}

/*When a battle is finish, compute xp gain and gain level*/
void finish_battle(int status, struct Game *game, struct Monster *monster) {
	if(status == WIN) {
		//gain d'xp
		int xp = ceil((monster->stats->xp*monster->stats->level*1.5)/7);

		dimage(42,DHEIGHT-75,&img_dialogue);

		dprint(50,DHEIGHT-75/2-10, C_BLACK, "Vous remportez %d points d'experience", xp);
		dupdate();
		wait_for_input(KEY_SHIFT);

		game->player->stats.xp += xp;

		//niveau suivant une progession N³
		int calc_level = (int)pow(game->player->stats.xp, 0.33);
		for(int i = game->player->stats.level; i < calc_level; i++) {
			draw_battle(game->player, monster);
			dimage(42,DHEIGHT-75,&img_dialogue);
			dprint(50,DHEIGHT-75/2-10,C_BLACK,"Vous passez au niveau %d !", i+1);
			dupdate();
			wait_for_input(KEY_SHIFT);
		}
		game->player->stats.level = calc_level;
		set_stats_level_from(&game->player->base_stats, &game->player->stats);

	} else if(status == LOSE) {
		game->player->stats.pv = 0;
	}

	free_monster(monster);
}

/**
 * Select a move in the list
 */
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

		if(keydown(KEY_SHIFT) && player->moves[selection]->pp > 0) {
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
	extern bopti_image_t img_battle;
	const int WIDTH_HP = 74;

	if(player->stats.pv < 0) player->stats.pv = 0;
	if(monster->stats->pv < 0) monster->stats->pv = 0;

	dclear(C_RGB(25,25,25));
	dimage(0,0,&img_battle);
	int posHP = (float)player->stats.pv / player->stats.max_pv * WIDTH_HP;
	drect(290,138,290+WIDTH_HP,142,C_WHITE);

	int pcolor = (player->stats.pv <= player->stats.max_pv/2 ? 
		(player->stats.pv <= player->stats.max_pv/4 ? 
			C_RED : 
			C_RGB(31,17,0)) : 
		C_GREEN);

	drect(290,138,290+posHP,142,pcolor);

	dprint(333,124,C_BLACK,"%d",player->stats.level);
	dprint(246,124,C_BLACK,"%d/%d", player->stats.pv, player->stats.max_pv);

	int posHPmonster = (float)monster->stats->pv / monster->stats->max_pv * WIDTH_HP;
	dprint(2,8,C_BLACK,"%s",monster->name);
	drect(48,23,48+WIDTH_HP,27,C_WHITE);

	int mcolor = (monster->stats->pv <= monster->stats->max_pv/2 ? 
		(monster->stats->pv <= monster->stats->max_pv/4 ? 
			C_RED : 
			C_RGB(31,17,0)) : 
		C_GREEN);

	drect(48,23,48+posHPmonster,27,mcolor);
	dprint(90,9,C_BLACK,"%d",monster->stats->level);
	dprint(127,11,C_BLACK,"%d/%d", monster->stats->pv, monster->stats->max_pv);

	dimage(265,10,monster->sprite);
}

void draw_executed_move(struct Move *move, struct Monster *monster, int is_monster) {
	dimage(42,DHEIGHT-75,&img_dialogue);
	if(is_monster) {
		dprint(50,DHEIGHT-75/2-10, C_BLACK, "%s lance %s !", monster->name, move->name);
	} else {
		dprint(50,DHEIGHT-75/2-10, C_BLACK, "Vous lancez %s !", move->name);
	}
}

void draw_crit() {
	dimage(42,DHEIGHT-75,&img_dialogue);
	dprint(50,DHEIGHT-75/2-10, C_BLACK, "Coup critique !");
}