#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>

#include "talkable.h"
#include "util.h"

#define LIMIT 35

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

		//TODO Il y a moyen de faire mieux avec seulement curr_line.
		//Cela semble globalement fonctionner, mais j'ai pu observer des petites erreurs
		if(strlen(curr_line) > LIMIT) {
			char remain[strlen(curr_line)+1];
			char split[LIMIT+1];
			char first_part[LIMIT+1];

			strcpy(remain, curr_line);
			int indexLine = 0;

			while(strlen(remain) > LIMIT) {	
				clean(split);
				strncpy(first_part,&remain[0],LIMIT);

				char* find = strrchr(first_part,' ');
				int indexSpace = find != NULL ? (int)(find-first_part) : LIMIT-1;
				strncpy(split,&remain[0],indexSpace);
				strncpy(remain,&remain[indexSpace]+1,strlen(remain));
				
				dprint(25,40+indexLine*15, C_BLACK, "%s", split);
				indexLine++;
			}
			dprint(25,40+indexLine*15, C_BLACK, "%s", remain);
		} else {
			dprint(25,40, C_BLACK, "%s", curr_line);
		}

		dupdate();
		curr_line = strtok(NULL, delim);
		wait_for_input(KEY_SHIFT);
	}
}

void clean(char *var) {
    int i = 0;
    while(var[i] != '\0') {
        var[i] = '\0';
        i++;
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