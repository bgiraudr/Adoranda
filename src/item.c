#include "item.h"
#include "player.h"
#include "util.h"
#include <gint/keyboard.h>
#include <gint/display.h>
#include <stdbool.h>
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

bool select_item(struct Game *game, int pos) {
    const char *delim = ";";
    char *str = strdup(game->player->inventory.items[pos]->action);
	char *curr_line = strtok(str, delim);
	bool ret = false;
    while(curr_line != NULL) {
        if(handle_event(game, curr_line)) ret = true;
        curr_line = strtok(NULL, delim);
    }
	return ret;
}

struct Item *get_item_from_name(const char *name) {
    for(int i = 0; i < items.nbItems; i++) {
		if(!strcmp(items.items[i]->name, name)) return items.items[i];
	}
	return items.items[0];
}

int compare(struct Item *item1, struct Item *item2) {
	return strcmp(item1->name, item2->name);
}