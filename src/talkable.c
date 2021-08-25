#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include "talkable.h"
#include "engine.h"
#include "map.h"
#include "util.h"

struct Talkable default_value = {
	.x = 0,
	.y = 0,
	.name = "default name",
	.text = "default dialog"
};

/*draw the dialog of a specified talkable tile*/
void draw_dialog(struct Talkable *talkable) {
	const char *delim = ";";

	char *str = strdup(talkable->text);
	char *curr_line = strtok(str, delim);

	while(curr_line != NULL) {
		drect(20,10,370,80,C_WHITE);
		dprint(25,20, C_BLACK, "%s", talkable->name);
		dprint(25,40, C_BLACK, "%s", curr_line);
		dupdate();
		curr_line = strtok(NULL, delim);
		wait_for_input(KEY_SHIFT);
	}
}

/*find the talkable tile using the player's position*/
struct Talkable* get_dialog_xy(struct Map *map, int x, int y) {
	int i = 0;
	while(i < map->dialog_count) {
		if(map->dialogs[i].x == x && map->dialogs[i].y == y) return &map->dialogs[i];
		i++;
	}
	return &default_value;
}