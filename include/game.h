#pragma once

void play();
int get_inputs(void);

enum direction {
	DIR_DOWN = 0,
	DIR_LEFT = 1,
	DIR_UP = 2,
	DIR_RIGHT = 3,
	ACTION_SHIFT = 4
};