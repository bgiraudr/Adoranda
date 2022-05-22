#pragma once

/*wait for a specified input key*/
void wait_for_input(int input);
int rand_range(int low, int high);
void format_text_opt(int x, int y, int width, int height, const int color, char const *format, ...);
char *word_boundary_before(char *str, char *limit);
char *skip_spaces(char *str);
int yes_no_question(char const *format, ...);