#pragma once

/*get the input with a timeout*/
int get_inputs(void);

enum direction {
	DIR_DOWN = 0,
	DIR_LEFT = 1,
	DIR_UP = 2,
	DIR_RIGHT = 3,
	ACTION_SHIFT = 4,
	ACTION_ALPHA = 5
};
