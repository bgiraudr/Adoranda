#pragma once
#include "define.h"
#include <gint/display.h>

struct Item {
    char *name;
    int id;
    bopti_image_t *sprite;
    char *description;
};

struct Items {
    int nbItems;
    struct Item *items[];
};

struct Inventory {
    int nbItems;
    struct Item *items[NB_PLAYER_ITEMS];
};

struct Item *get_item_id(int id);
bool add_item_to_inventory(struct Inventory *inventory, struct Item *item);
int get_first_free_space(struct Inventory *inventory);
void remove_item_pos(struct Inventory *inventory, int pos);
void display_inventory(struct Inventory *inventory);