#include <stdio.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdbool.h>

#include "inventory.h"
#include "util.h"
#include "define.h"
#include "vec2.h"
#include "talkable.h"
#include "event.h"
#include "item.h"
#include "draw_util.h"
#include "game.h"

int get_first_free_space(struct Inventory *inventory) {
    for(int i = 0; i < NB_PLAYER_ITEMS; i++) {
		if(inventory->items[i] == NULL) {
			return i;
		}
	}
	return NB_PLAYER_ITEMS;
}

int get_nb_items(struct Inventory *inventory) {
    int ret = 0;
    for(int i = 0; i < NB_PLAYER_ITEMS; i++) {
		if(inventory->items[i] != NULL) {
			ret++;
		}
	}
	return ret;
}

void sort_inventory(struct Inventory *inventory) {
    const int n = NB_PLAYER_ITEMS;
    int i = 0, j = 0;
    struct Item *tmp;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - i - 1; j++) {
        	if (inventory->items[j+1] != NULL) {
        		if (inventory->items[j] != NULL) {
		            if (compare(inventory->items[j], inventory->items[j+1]) > 0) {
		                tmp = inventory->items[j];
		                inventory->items[j] = inventory->items[j + 1];
		                inventory->items[j + 1] = tmp;
		            }
		       	} else {
		        	tmp = inventory->items[j];
	                inventory->items[j] = inventory->items[j + 1];
	                inventory->items[j + 1] = tmp;
	        	}
        	}
	    }
	}
}

bool add_item_to_inventory(struct Game *game, struct Inventory *inventory, struct Item *item) {
    int index = get_first_free_space(inventory);

    if(index < NB_PLAYER_ITEMS) {
        draw_text(50, DHEIGHT-47, C_BLACK, "Vous ajoutez %s à votre inventaire !", item->name);
        dupdate();
        wait_for_input(KEY_SHIFT);
        inventory->items[index] = item;
        inventory->nbItems++;
        return true;
    } else {
        draw_text(50, DHEIGHT-47, C_BLACK, "Plus de place pour ajouter %s à votre inventaire !", item->name);
        dupdate();
        wait_for_input(KEY_SHIFT);
        int pos = open_inventory(game, inventory, "Remplacer", false);
        if(pos != -1) { 
            inventory->items[pos] = item;
            inventory->nbItems++;
            return true;
        }
    }
    return false;
}

void display_inventory(struct Inventory *inventory) {
    const int x_inventory = 30;
    const int y_inventory = 56;
    for(int i = 0 ; i < NB_PLAYER_ITEMS ; i++) {
        int x = i%10;
        int y = i/10;
        if(inventory->items[i] != NULL) {
            dimage(34*x+x_inventory, 41*y+y_inventory, inventory->items[i]->sprite);
        } else {
            drect(34*x+x_inventory, 41*y+y_inventory, 34*x+x_inventory+31, 41*y+y_inventory+31, C_BLACK);
        }
    }
}

int open_inventory(struct Game *game, struct Inventory *inventory, char* context, bool keep_open) {
    extern bopti_image_t img_inventory;
	int buffer = keydown(KEY_SHIFT);
	struct Vec2 cursor = VEC2(0,0);
    int pos = 0;
    int suppression = 0;

    const int x_inventory = 30;
    const int y_inventory = 56;

	while(1) {
		clearevents();

		cursor.x += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
        cursor.y += keydown(KEY_DOWN) - keydown(KEY_UP);
		
		if(cursor.x > 9) cursor.x = 9;
		if(cursor.x < 0) cursor.x = 0;
        if(cursor.y > 2) cursor.y = 2;
		if(cursor.y < 0) cursor.y = 0;

        pos = cursor.x + cursor.y*10;

		dclear(C_RGB(25,25,25));
        dimage(0, 0, &img_inventory);
        dtext(5,2,C_BLACK,context);
        drect(34*cursor.x+x_inventory-2, 41*cursor.y+y_inventory-2, 34*cursor.x+x_inventory+33, 41*cursor.y+y_inventory+33, suppression ? C_RED : C_GREEN);
        if(suppression) dtext(5,16,C_RED,"Suppression");
        dprint(5, 185, C_BLACK, "%d/%d", get_nb_items(inventory), NB_PLAYER_ITEMS);
		display_inventory(inventory);
        
        if(inventory->items[pos] != NULL) {
            dtext(5, 30, C_BLACK, inventory->items[pos]->name);
		    dtext(104, 30, C_BLACK, inventory->items[pos]->description);
        }
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else if(inventory->items[pos] != NULL) {
                if(!suppression && !keep_open) break;
                else if(!suppression) {
                    if(select_item(game, pos)) remove_item_pos(inventory, pos);
                    else {
                        draw_text(50, DHEIGHT-47, C_BLACK, "Ça n'aura aucun effet.");
                        dupdate();
                        wait_for_input(KEY_SHIFT);
                    }
                }
                else remove_item_pos(inventory, pos);
            }
		}
        if(keydown(KEY_OPTN)) {
            suppression = suppression ? 0 : 1;
		}
        if(keydown(KEY_F6)) {
            sort_inventory(inventory);
		}
		if(keydown(KEY_EXIT)) {
            pos = -1;
			break;
		}
		while(keydown_any(KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_SHIFT, KEY_OPTN, KEY_F6, 0)) clearevents();
	}
	return pos;
}