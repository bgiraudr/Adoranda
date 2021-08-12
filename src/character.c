#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include "character.h"

/*draw the dialog of a specified character*/
void draw_dialog(struct character *character) {
	drect(20,10,370,80,C_WHITE);
	dprint(25,20, C_BLACK, "(%d,%d)", character->x, character->y);
	dprint(25,40, C_BLACK, "%s", character->name);
	dprint(25,60, C_BLACK, "%s", character->dialog);
	dupdate();
	int buffer = 1;
	while(1) {
		clearevents();
		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else break;
		}
		while(keydown(KEY_SHIFT)) clearevents();
	}
}

/*find the character using the player's position*/
struct character* get_character_xy(struct character *characters[], int x, int y) {
	int i = 0;
	while(strcmp(characters[i]->name,"default name") != 0) {
		if(characters[i]->x == x && characters[i]->y == y) return characters[i];
		i++;
	}
	return characters[i];
}