#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include "character.h"
#include "engine.h"
#include "map.h"

struct Character character_default = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.dialog = "default dialog"
};

/*draw the dialog of a specified character*/
void draw_dialog(struct Character *character) {
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
struct Character* get_character_xy(struct Character *characters[], int x, int y) {
	int i = 0;
	while(strcmp(characters[i]->name,"default name") != 0) {
		if(characters[i]->x == x && characters[i]->y == y) return characters[i];
		i++;
	}
	return &character_default;
}

/*get the characters for a specified map*/
struct Character** get_map_characters(int id) {
	if(id == 1) {
		extern struct Character character_Tituya;
		extern struct Character character_Lephenixnoir;
		extern struct Character character_Tituya2;
		extern struct Character character_KikooDX;
		extern struct Character character_Massena;
		extern struct Character character_PancarteVille;

		static struct Character *characters[] = {
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
	static struct Character *characters[] = {};
	return characters;
}
