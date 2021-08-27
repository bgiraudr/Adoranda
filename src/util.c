#include <gint/keyboard.h>
#include <gint/std/stdlib.h>

#include "util.h"

/*wait for a specified input key*/
void wait_for_input(int input) {
	int buffer = 1;
	while(1) {
		clearevents();
		if(keydown(input)) {
			if(buffer) buffer = 0;
			else break;
		}
		while(keydown(input)) clearevents();
	}
}

int rand_range(int low, int high) { return (rand() % (high - low)) + low; }