struct character {
	int x, y;
	char *name;
	char *dialog;
};

void draw_dialog(struct character *character);
struct character* get_character_xy(struct character *characters[], int x, int y);