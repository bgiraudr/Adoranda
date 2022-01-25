#include <gint/display.h>
#include <gint/keyboard.h>

#include "engine.h"
#include "battle.h"
#include "util.h"
#include "capacite.h"
#include "player.h"

void create_battle(struct Player *player) {
	player->stats.pv = player->stats.max_pv;
	during_battle(player);
}

int during_battle(struct Player *player) {
	int tour = 0;

	int selection = 0;
	while(1) {
		int buffer = keydown(KEY_SHIFT);
		while(1) {
			clearevents();

			if(player->moves[1].name != NULL) {
				selection += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
			}
			
			if(selection > 1) selection = 1;
			if(selection < 0) selection = 0;

			dclear(C_RGB(25,25,25));
			draw_ui(player);
			dtext(95 + (selection * 190), DHEIGHT-15 , C_RED, "[X]");
			dupdate();

			if(keydown(KEY_SHIFT)) {
				if(buffer) buffer = 0;
				else break;
			}
			if(keydown(KEY_EXIT)) {
				player->stats.pv--;
				break;
			}
			while(keydown(KEY_SHIFT)) clearevents();
		}

		execute_move(&player->stats, player->moves[selection]);
		if(player->stats.pv <= 0) {
			return LOSE;
		}
		tour++;
	}
	return LOSE;
}