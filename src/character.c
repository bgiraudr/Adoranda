#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include "character.h"
#include "engine.h"
#include "map.h"

struct character character_default = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.dialog = "default dialog"
};

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
	return &character_default;
}

struct character** get_map_characters(int id) {
	if(id == 1) {
		extern struct character character_Tituya;
		extern struct character character_Lephenixnoir;
		extern struct character character_Tituya2;
		extern struct character character_KikooDX;
		extern struct character character_Massena;
		extern struct character character_PancarteVille;

		static struct character *characters[] = {
			&character_Tituya,
			&character_Lephenixnoir,
			&character_Massena,
			&character_Tituya2,
			&character_KikooDX,
			&character_PancarteVille,
			&character_default,
		};
		return characters;
	}
	static struct character *characters[] = {};
	return characters;
}