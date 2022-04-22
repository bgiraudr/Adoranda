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

#define LIMIT 35

struct Talkable default_value = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.text = "default dialog"
};

/* Find the last word boundary between [str] and [limit]. If there is no word
   boundary (ie. [limit] is in the first word of [str]), returns [limit].
   This function always return a position *after* [str] so that we always make
   progress and don't loop. */
char *word_boundary_before(char *str, char *limit)
{
	char *s = limit;
	while(s > str) {
		if(isspace(*s) || !*s) return s;
		s--;
	}
	return limit;
}

/* Skip spaces at the start of [str] */
char *skip_spaces(char *str)
{
	return str + strspn(str, " \t\n");
}

/*draw the dialog of a specified talkable tile*/
void draw_dialog(struct Game *game) {
	const char *delim = ";";

	int direction = game->player->direction;
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);
	struct Talkable *talk = get_dialog_xy(game->map, game->player->pos.x + dx, game->player->pos.y + dy);

	char *str = strdup(talk->text);
	char *curr_line = strtok(str, delim);

	while(curr_line != NULL) {
		char *event = strchr(curr_line, '~');
		if(event) handle_event(game, curr_line+1);
		else {
			format_text(50, 58, C_BLACK, curr_line);
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

void format_text(int x, int y, const int color, char const *format, ...) {
	int const DIALOG_WIDTH = 295, LINE_HEIGHT = 13;
	extern bopti_image_t img_dialogue;
    dimage(x-7,y-26,&img_dialogue);

	char text_arg[512];
	va_list args;
	va_start(args, format);
	vsnprintf(text_arg, 512, format, args);
	va_end(args);

	char *text = (char *)malloc(strlen(text_arg)+1);
    strcpy(text,text_arg);

	while(*text) {
		char *end = (char *)drsize(text, NULL, DIALOG_WIDTH, NULL);
		char *last_word = word_boundary_before(text, end);
		dtext_opt(x, y, color, C_NONE, DTEXT_LEFT, DTEXT_TOP,
			text, last_word - text);
		text = skip_spaces(last_word);
		y += LINE_HEIGHT;
	}
}