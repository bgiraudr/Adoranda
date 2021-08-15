#pragma once

struct character {
	/*the position of the character*/
	int x, y;
	/*the name*/
	char *name;
	/*the text to say*/
	char *dialog;
};

/*draw the dialog of a specified character*/
void draw_dialog(struct character *character);
/*find the character using the player's position*/
struct character* get_character_xy(struct character *characters[], int x, int y);
/*get the characters for a specified map*/
struct character** get_map_characters(int id);