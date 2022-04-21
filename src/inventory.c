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
#include "game.h"

int get_first_free_space(struct Inventory *inventory) {
    for(int i = 0; i < NB_PLAYER_ITEMS; i++) {
		if(inventory->items[i] == NULL) {
			return i;
		}
	}
	return NB_PLAYER_ITEMS;
}

bool add_item_to_inventory(struct Game *game, struct Inventory *inventory, struct Item *item) {
    int index = get_first_free_space(inventory);

    extern bopti_image_t img_dialogue;
    dimage(42,DHEIGHT-75,&img_dialogue);

    if(index < NB_PLAYER_ITEMS) {
        format_text(50, DHEIGHT-47, C_BLACK, "Vous ajoutez %s à votre inventaire !", item->name);
        dupdate();
        wait_for_input(KEY_SHIFT);
        inventory->items[index] = item;
        inventory->nbItems++;
        return true;
    } else {
        format_text(50, DHEIGHT-47, C_BLACK, "Plus de place pour ajouter %s à votre inventaire !", item->name);
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
    for(int i = 0 ; i < NB_PLAYER_ITEMS ; i++) {
        int x = i%10;
        int y = i/10;
        if(inventory->items[i] != NULL) {
            dimage(18*x+50,25*y+50,inventory->items[i]->sprite);
        } else {
            drect(18*x+50,25*y+50,18*x+65,25*y+65,C_BLACK);
        }
    }
}

int open_inventory(struct Game *game, struct Inventory *inventory, char* context, bool keep_open) {
	int buffer = keydown(KEY_SHIFT);
	struct Vec2 cursor = VEC2(0,0);
    int pos = 0;
    int suppression = 0;

	while(1) {
		clearevents();

		cursor.x += keydown(KEY_RIGHT) - keydown(KEY_LEFT);
        cursor.y += keydown(KEY_DOWN) - keydown(KEY_UP);
		
		if(cursor.x > 9) cursor.x = 9;
		if(cursor.x < 0) cursor.x = 0;
        if(cursor.y > 2) cursor.y = 2;
		if(cursor.y < 0) cursor.y = 0;

        pos = cursor.x + cursor.y*10;

		dclear(C_WHITE);
        dtext(130,15,C_BLACK,context);
        drect(18*cursor.x+48, 25*cursor.y+48, 18*cursor.x+67, 25*cursor.y+67,suppression ? C_RED : C_GREEN);
        if(suppression) dtext(130,35,C_RED,"Suppression");
		display_inventory(inventory);
        
        if(inventory->items[pos] != NULL) {
            dprint(10, DHEIGHT-30, C_BLACK, "nom : %s", inventory->items[pos]->name);
		    dprint(10, DHEIGHT-15, C_BLACK, "desc : %s", inventory->items[pos]->description);
        }
		dupdate();

		if(keydown(KEY_SHIFT)) {
			if(buffer) buffer = 0;
			else if(inventory->items[pos] != NULL) {
                if(!suppression && !keep_open) break;
                else if(!suppression) {
                    select_item(game, pos);
                    remove_item_pos(inventory, pos);
                }
                else remove_item_pos(inventory, pos);
            }
		}
        if(keydown(KEY_OPTN)) {
            suppression = suppression ? 0 : 1;
		}
		if(keydown(KEY_EXIT)) {
            pos = -1;
			break;
		}
		while(keydown_any(KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_SHIFT, KEY_OPTN, 0)) clearevents();
	}
	return pos;
}