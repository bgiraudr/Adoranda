#pragma once

struct Character {
	/*the position of the character*/
	int x, y;
	/*the name*/
	char *name;
	/*the text to say*/
	char *dialog;
};

/*draw the dialog of a specified character*/
void draw_dialog(struct Character *character);
/*find the character using the player's position*/
struct Character* get_character_xy(struct Character *characters[], int x, int y);
/*get the characters for a specified map*/
struct Character** get_map_characters(int id);
