#pragma once
#include "define.h"
#include "game.h"
#include <stdbool.h>

struct Inventory {
    int nbItems;
    struct Item *items[NB_PLAYER_ITEMS];
};


bool add_item_to_inventory(struct Game *game, struct Inventory *inventory, struct Item *item);
int get_first_free_space(struct Inventory *inventory);
void display_inventory(struct Inventory *inventory);
int open_inventory(struct Game *game, struct Inventory *inventory, char* context, bool keep_open);
int get_nb_items(struct Inventory *inventory);
void sort_inventory(struct Inventory *inventory);