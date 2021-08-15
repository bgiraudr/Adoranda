#pragma once

struct character {
	/*the position of the character*/
	int x, y;
	/*the name*/
	char *name;
	/*the text to say*/
	char *dialog;
};

void draw_dialog(struct character *character);
struct character* get_character_xy(struct character *characters[], int x, int y);
struct character** get_map_characters(int id);