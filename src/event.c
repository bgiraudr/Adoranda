#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "event.h"
#include "player.h"
#include "capacite.h"
#include "inventory.h"

bool handle_event(struct Game *game, char const *event)
{
    if(!strncmp(event, "give:", 5)) {
        event += 5;
        // Isoler le nom et la quantité
        int len=strlen(event), qty=1;
        char *star = strchr(event, '*');
        if(star) len=star-event, qty=atoi(star+1);
        add_item_to_inventory(&game->player->inventory,get_item_id(2));
        return true;
    }
    else if(!strncmp(event, "xp:", 3)) {
    	event += 3;
    	add_xp(game->player, atoi(event));
    	return true;
    }
    else if(!strncmp(event, "move:", 5)) {
    	event += 5;
    	add_move(game->player, get_move_id(atoi(event)));
    	return true;
    }
    else if(!strcmp(event, "test:test")) {
        // etc, n'importe quoi de compliqué
        return true;
    }
    return false;
}