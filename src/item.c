#include "item.h"
#include "player.h"
#include "util.h"
#include <gint/keyboard.h>
#include <gint/display.h>
#include <string.h>
#include "event.h"

extern struct Items items;

struct Item *get_item_id(int id) {
	for(int i = 0; i < items.nbItems; i++) {
		if(items.items[i]->id == id) return items.items[i];
	}
	return items.items[0];
}

void remove_item_pos(struct Inventory *inventory, int pos) {
    inventory->items[pos] = NULL;
	inventory->nbItems--;
}

void select_item(struct Game *game, int pos) {
    const char *delim = ";";
    char *str = strdup(game->player->inventory.items[pos]->action);
	char *curr_line = strtok(str, delim);
    while(curr_line != NULL) {
        handle_event(game, curr_line);
        curr_line = strtok(NULL, delim);
    }
}

struct Item *get_item_from_name(const char *name) {
    for(int i = 0; i < items.nbItems; i++) {
		if(!strcmp(items.items[i]->name, name)) return items.items[i];
	}
	return items.items[0];
}