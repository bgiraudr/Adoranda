#pragma once
#include "map.h"
#include "game.h"
#include <stdbool.h>

struct Talkable {
	/*the position of the tile*/
	int x, y, id;
	/*the name*/
	char *name;
	/*the text to display*/
	char *text;
	/*the text to display after the action*/
	char *text_after;
	/*if event, repeat it or no*/
	int exclusive;
};

/*draw the dialog of a specified talkable tile*/
void draw_dialog(struct Game *game);
/*find the talkable tile using the player's position*/
struct Talkable* get_dialog_xy(struct Map *map, int x, int y);
void addDialogToPlayer(struct Player *player, int id);
void draw_dialog_text(struct Game *game, char *text);