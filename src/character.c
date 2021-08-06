#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include "character.h"

void draw_dialog(struct character *character) {
	dprint(1,20, C_BLACK, "(%d,%d)", character->x, character->y);
	dprint(1,40, C_BLACK, "%s", character->name);
	dprint(1,60, C_BLACK, "%s", character->dialog);
}

struct character* get_character_xy(struct character *characters[], int x, int y) {
	int i = 0;
	while(strcmp(characters[i]->name,"default name") != 0) {
		if(characters[i]->x == x && characters[i]->y == y) return characters[i];
		i++;
	}
	return characters[i];
}