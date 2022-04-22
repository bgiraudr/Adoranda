#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "event.h"
#include "player.h"
#include "capacite.h"
#include "inventory.h"
#include "item.h"

bool handle_event(struct Game *game, char const *event)
{
    if(!strncmp(event, "give:", 5)) {
        event += 5;
        // Isoler le nom et la quantité
        int len=strlen(event), qty=1;
        char *star = strchr(event, '*');
        char name[20];
        if(star) len=star-event, qty=atoi(star+1);
        strncpy(name,event,len);
        name[len] = '\0';
        while(qty) {
            add_item_to_inventory(game, &game->player->inventory, get_item_from_name(name));
            qty--;
        }
        return true;
    }
    else if(!strncmp(event, "xp:", 3)) {
    	event += 3;
    	add_xp(game->player, atoi(event));
    	return true;
    }
    else if(!strncmp(event, "hp:", 3)) {
    	event += 3;
    	game->player->stats.pv += atoi(event);
        if(game->player->stats.pv > game->player->stats.max_pv) game->player->stats.pv = game->player->stats.max_pv;
    	return true;
    }
    else if(!strcmp(event, "pp:all")) {
        reset_pp(game->player);
        return true;
    }
    else if(!strncmp(event, "pp:", 3)) {
    	event += 3;
    	add_pp(game->player, atoi(event));
    	return true;
    }
    else if(!strncmp(event, "move:", 5)) {
    	event += 5;
    	add_move(game->player, get_move_id(atoi(event)));
    	return true;
    }
    return false;
}