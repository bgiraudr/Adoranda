#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/cpu.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "talkable.h"
#include "util.h"
#include "event.h"
#include "player.h"
#include "draw_util.h"

struct Talkable default_value = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.text = "default dialog",
	.text_after = "",
	.exclusive = 0
};

/*draw the dialog of a specified talkable tile*/
void draw_dialog(struct Game *game) {
	const char *delim = ";";

	int direction = game->player->direction;
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);
	struct Talkable *talk = get_dialog_xy(game->map, game->player->pos.x + dx, game->player->pos.y + dy);

	char *str = strdup(talk->text);
	if(talk->exclusive) {
		if(check_eventdialog(game->player, talk->id)) {
			str = strdup(talk->text_after);
		} else {
			addDialogToPlayer(game->player, talk->id);
		}
	}
	char *curr_line = strtok(str, delim);

	while(curr_line != NULL) {
		char *event = strchr(curr_line, '~');
		if(event) handle_event(game, curr_line+1);
		else {
			draw_text(50, 58, C_BLACK, curr_line);
			dprint(50,40, C_BLACK, "%s", talk->name);

			dupdate();
			wait_for_input(KEY_SHIFT);
		}
		curr_line = strtok(NULL, delim);
	}
}

/*find the talkable tile using the player's position*/
struct Talkable* get_dialog_xy(struct Map *map, int x, int y) {
	int i = 0;
	while(i < map->dialog_count) {
		if(map->dialogs[i].x == x && map->dialogs[i].y == y) return &map->dialogs[i];
		i++;
	}
	return &default_value;
}

void addDialogToPlayer(struct Player *player, int id) {
	player->eventListDialog[get_nb_eventdialog(player)] = id;
}