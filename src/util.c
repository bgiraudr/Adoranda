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