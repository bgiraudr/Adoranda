#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdbool.h>

#include "inventory.h"
#include "util.h"
#include "define.h"

extern struct Items items;

struct Item *get_item_id(int id) {
	for(int i = 0; i < items.nbItems; i++) {
		if(items.items[i]->id == id) return items.items[i];
	}
	return items.items[0];
}

int get_first_free_space(struct Inventory *inventory) {
    for(int i = 0; i < NB_PLAYER_ITEMS; i++) {
		if(inventory->items[i] == NULL) {
			return i;
		}
	}
	return NB_PLAYER_ITEMS;
}

bool add_item_to_inventory(struct Inventory *inventory, struct Item *item) {
    int index = get_first_free_space(inventory);
    if(index < NB_PLAYER_ITEMS) {
        inventory->items[index] = item;
        inventory->nbItems++;
        return true;
    }
    return false;
}

void remove_item_pos(struct Inventory *inventory, int pos) {
	inventory->items[pos] = NULL;
	inventory->nbItems--;
}

void display_inventory(struct Inventory *inventory) {
    dclear(C_WHITE);
    for(int i = 0 ; i < NB_PLAYER_ITEMS ; i++) {
        if(inventory->items[i] != NULL) {
            dimage(0,18*i,inventory->items[i]->sprite);
        }
    }
    dupdate();
    wait_for_input(KEY_SHIFT);
}