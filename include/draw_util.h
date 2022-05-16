#pragma once

void draw_text(int x, int y, const int color, char const *format, ...);
void draw_change_one_color(int x, int y, bopti_image_t *image, uint16_t from, uint16_t to);
void draw_change_one_color_index(int x, int y, bopti_image_t *image, int index, uint16_t color);