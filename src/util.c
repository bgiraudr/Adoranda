#include <gint/keyboard.h>
#include <gint/display.h>
#include <stdlib.h>
#include <string.h>
#include <gint/cpu.h>
#include <ctype.h>
#include <stdio.h>

#include "util.h"

/*wait for a specified input key*/
void wait_for_input(int input) {
	int buffer = keydown(input);
	while(1) {
		clearevents();
		if(keydown(input)) {
			if(buffer) buffer = 0;
			else break;
		}
		while(keydown(input)) clearevents();
	}
}

/**
 * Random [low;high[
 */
int rand_range(int low, int high) { return (rand() % (high - low)) + low; }

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

void format_text_opt(int x, int y, int width, int height, const int color, char const *format, ...) {
	int const DIALOG_WIDTH = width, LINE_HEIGHT = height;

	char text_arg[512];
	va_list args;
	va_start(args, format);
	vsnprintf(text_arg, 512, format, args);
	va_end(args);

	char *text = (char *)malloc(strlen(text_arg)+1);
    strcpy(text, text_arg);

	while(*text) {
		char *end = (char *)drsize(text, NULL, DIALOG_WIDTH, NULL);
		char *last_word = word_boundary_before(text, end);
		dtext_opt(x, y, color, C_NONE, DTEXT_LEFT, DTEXT_TOP,
			text, last_word - text);
		text = skip_spaces(last_word);
		y += LINE_HEIGHT;
	}
}

int yes_no_question(char const *format, ...) {
	char text_arg[512];
	va_list args;
	va_start(args, format);
	vsnprintf(text_arg, 512, format, args);
	va_end(args);

	int selection = 0;
	int buffer = keydown(KEY_SHIFT);
	while(1) {
		clearevents();
		dclear(C_WHITE);

		selection += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
		if(selection > 1) selection = 1;
		if(selection < 0) selection = 0;

		format_text_opt(95,10, 200, 13, C_BLACK, text_arg);
		
		dtext(95,150,C_BLACK, "NON");
		dtext(285,150,C_BLACK, "OUI");

		dtext(95 + (selection * 190), DHEIGHT-47, C_RED, "[X]");
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		if(keydown(KEY_EXIT)) {
			selection = 0;
			break;
		}
		while(keydown_any(KEY_LEFT,KEY_RIGHT, KEY_SHIFT,0)) clearevents();
	}
	return selection;
}