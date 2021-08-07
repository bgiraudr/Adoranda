#include <gint/display.h>
#include <gint/keyboard.h>
#include "game.h"
#include "player.h"

/*get the input with a timeout*/
int get_inputs(void) {
	int opt = GETKEY_DEFAULT & GETKEY_REP_ARROWS;
	int timeout = 1;

    while(1) {

    	key_event_t ev = getkey_opt(opt, &timeout);
		if(ev.type == KEYEV_NONE) return -1;
        int key = ev.key;

        /*direction*/
        if(key == KEY_DOWN)  return DIR_DOWN;
        if(key == KEY_RIGHT) return DIR_RIGHT;
        if(key == KEY_UP)    return DIR_UP;
        if(key == KEY_LEFT)  return DIR_LEFT;

        /*action key*/
        if(key == KEY_SHIFT)  return ACTION_SHIFT;
        if(key == KEY_ALPHA)  return ACTION_ALPHA;
    }
}