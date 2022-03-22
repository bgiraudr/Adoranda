#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/cpu.h>
#include <string.h>
#include <ctype.h>

#include "talkable.h"
#include "util.h"

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
void draw_dialog(struct Talkable *talkable) {
	extern bopti_image_t img_dialogue;
	const char *delim = ";";

	char *str = strdup(talkable->text);
	char *curr_line = strtok(str, delim);

	while(curr_line != NULL) {
		dimage(43,31,&img_dialogue);
		dprint(50,40, C_BLACK, "%s", talkable->name);

		int const DIALOG_WIDTH = 295, LINE_HEIGHT = 13;
		int y = 45 + LINE_HEIGHT;

		while(*curr_line) {
			char *end = (char *)drsize(curr_line, NULL, DIALOG_WIDTH, NULL);
			char *last_word = word_boundary_before(curr_line, end);
			dtext_opt(50, y, C_BLACK, C_NONE, DTEXT_LEFT, DTEXT_TOP,
				curr_line, last_word - curr_line);
			curr_line = skip_spaces(last_word);
			y += LINE_HEIGHT;
		}

		dupdate();
		curr_line = strtok(NULL, delim);
		wait_for_input(KEY_SHIFT);
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