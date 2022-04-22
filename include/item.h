#pragma once
#include "game.h"
#include "inventory.h"
#include <gint/display.h>

struct Item {
    char *name;
    int id;
    char *description;
    bopti_image_t *sprite;
    char *action;
};

struct Items {
    int nbItems;
    struct Item *items[];
};

struct Item *get_item_id(int id);
void select_item(struct Game *game, int pos);
void remove_item_pos(struct Inventory *inventory, int pos);
struct Item *get_item_from_name(const char *name);