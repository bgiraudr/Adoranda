#include <string.h>
#include <gint/keyboard.h>
#include <gint/display.h>
#include <gint/image.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "draw_util.h"
#include "util.h"

void draw_text(int x, int y, const int color, char const *format, ...) {
	int const DIALOG_WIDTH = 295, LINE_HEIGHT = 13;
	extern bopti_image_t img_dialogue;
    dimage(x-7,y-26,&img_dialogue);

	char text_arg[512];
	va_list args;
	va_start(args, format);
	vsnprintf(text_arg, 512, format, args);
	va_end(args);

	format_text_opt(x, y, DIALOG_WIDTH, LINE_HEIGHT, color, text_arg);
}

void draw_change_one_color(int x, int y, bopti_image_t *image, uint16_t from, uint16_t to) {
	for (int i = 0; i < image->color_count; i++) {
		if(image->palette[i] == from) {
			image->palette[i] = to;
			dimage(x, y, image);
			image->palette[i] = from;
			return;
		}
	}
}

void draw_change_one_color_index(int x, int y, bopti_image_t *image, int index, uint16_t color) {
	if(index >= image->color_count) return;
	uint16_t init_color = image->palette[index];
	image->palette[index] = color;
	dimage(x, y, image);
	image->palette[index] = init_color;
}