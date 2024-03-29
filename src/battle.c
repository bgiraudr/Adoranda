#include <gint/display.h>
#include <gint/keyboard.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"
#include "monster.h"
#include "talkable.h"
#include "draw_util.h"

extern bopti_image_t img_dialogue;

/**
 * create a new battle
 */
void create_battle(struct Game *game) {
	// game->player->stats.pv = game->player->stats.max_pv;
	// reset_pp(game->player);
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
	int status;
	while(1) {
		draw_battle(player, monster);
		dupdate();
		selection = select_move(player, monster, selection);
		draw_executed_move(player->moves[selection], monster, 0);
		dupdate();
		wait_for_input(KEY_SHIFT);
		status = execute_move(&player->stats, monster->stats, player->moves[selection], 0);
		check_move_status(status, player, monster, player->moves[selection], 0);

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
		status = execute_move(&player->stats, monster->stats, monster_move, 1);
		check_move_status(status, player, monster, monster_move, 1);

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

void check_move_status(int status, struct Player *player, struct Monster *monster, struct Move *move, int is_monster) {
	char *name = is_monster ? monster->name : "Player";
	if(status != SUCCESS) {
		draw_battle(player, monster);

        if(status == EFFECT) {
            char *boost_strings[5] = {"", "", "", "", ""};
            int boost_count = 0;

            if(move->boost_spe_atk > 0) boost_strings[boost_count++] = "son attaque spéciale";
            if(move->boost_spe_def > 0) boost_strings[boost_count++] = "sa défense spéciale";
            if(move->boost_atk > 0) boost_strings[boost_count++] = "son attaque";
            if(move->boost_def > 0) boost_strings[boost_count++] = "sa défense";
            if(move->boost_hp > 0) boost_strings[boost_count++] = "ses points de vie";

            if(boost_count > 0) {
                char boost_message[128] = "augmente ";
                for(int i = 0; i < boost_count; i++) {
                    if(i > 0) sprintf(boost_message + strlen(boost_message), (i == boost_count - 1) ? " et " : ", ");
                    sprintf(boost_message + strlen(boost_message), "%s", boost_strings[i]);
                }
                sprintf(boost_message + strlen(boost_message), " !");
                draw_status(name, boost_message);

                if(is_monster) self_effect(monster->stats, move);
                else self_effect(&player->stats, move);
            }
        } else {
            switch (status) {
                case MISS:
                    draw_status(name, "rate son attaque !");
                    break;
                case SUPER_EFFECTIVE:
                    draw_status(name, "utilise une attaque super efficace !");
                    break;
                case LESS_EFFECTIVE:
                    draw_status(name, "utilise une attaque peu efficace");
                    break;
                case NOT_EFFECTIVE:
                    draw_status(name, "utilise une attaque inefficace sur le type adverse !");
                    break;
            }
        }
		
		dupdate();
		wait_for_input(KEY_SHIFT);
	}
	if(is_crit() && (status == LESS_EFFECTIVE || status == SUPER_EFFECTIVE)) {
		draw_status(name, "réalise un coup critique !");
		dupdate();
		wait_for_input(KEY_SHIFT);
	}
}

/*When a battle is finish, compute xp gain and gain level*/
void finish_battle(int status, struct Game *game, struct Monster *monster) {
	if(status == WIN) {
		//gain d'xp
		int xp = ceil((monster->stats->xp*monster->stats->level*1.5)/7);

		draw_text(50, DHEIGHT-47, C_BLACK, "Vous remportez %d points d'experience", xp);
		dupdate();
		wait_for_input(KEY_SHIFT);

		add_xp(game->player, xp);

	} else if(status == LOSE) {
		draw_battle(game->player, monster);
		draw_text(50, DHEIGHT-47, C_BLACK, "%s a eu raison de vous !", monster->name);
		dupdate();
		wait_for_input(KEY_SHIFT);
		game->player->stats.pv = 0;
	}

	//On retire les changements de status en revenant à la base
	set_stats_level_from(&game->player->base_stats, &game->player->stats);
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
		draw_ui(player, selection);
		dupdate();

		if(keydown(KEY_OPTN)) {
			draw_stats(player->stats);
			dupdate();
			wait_for_input(KEY_OPTN);
		}

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else if(player->moves[selection]->pp > 0) break;
		}
		if(keydown(KEY_EXIT)) {
			break;
		}
		while(keydown_any(KEY_RIGHT, KEY_LEFT, KEY_SHIFT, KEY_OPTN, 0)) clearevents();
	}
	return selection;
}

void draw_battle(struct Player *player, struct Monster *monster) {
	extern bopti_image_t img_battle;
	extern bopti_image_t img_battle_env_1;
	const int WIDTH_HP = 74;

	if(player->stats.pv < 0) player->stats.pv = 0;
	if(monster->stats->pv < 0) monster->stats->pv = 0;

	// dclear(C_RGB(25,25,25));
	dimage(0,0,&img_battle_env_1);
	drect(323, 120, DWIDTH, 125, getTypeFromName(player->stats.type).color);
	dimage(0,0,&img_battle);
	int posHP = (float)player->stats.pv / player->stats.max_pv * WIDTH_HP;
	drect(290,143,290+WIDTH_HP,147,C_WHITE);

	int pcolor = (player->stats.pv <= player->stats.max_pv/2 ? 
		(player->stats.pv <= player->stats.max_pv/4 ? 
			C_RED : 
			C_RGB(31,17,0)) : 
		C_GREEN);

	drect(290,143,290+posHP,147,pcolor);

	dprint(333,129,C_BLACK,"%d",player->stats.level);
	dprint(256,127,C_BLACK,"%d/%d", player->stats.pv, player->stats.max_pv);

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
	drect(0,0,84,4,getTypeFromName(monster->stats->type).color);
	
	dimage((int)(253+((133-monster->sprite_x)/2)),6+(116-monster->sprite_y), monster->sprite);
}

void draw_executed_move(struct Move *move, struct Monster *monster, int is_monster) {
	if(is_monster) {
		draw_text(50, DHEIGHT-47, C_BLACK, "%s lance %s !", monster->name, move->name);
	} else {
		draw_text(50, DHEIGHT-47, C_BLACK, "Vous lancez %s !", move->name);
	}
}

void draw_status(char *name, char *message) {
	draw_text(50, DHEIGHT-47, C_BLACK, "%s %s", name, message);
}