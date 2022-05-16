#include <stdbool.h>
#include <string.h>
#include "type.h"

#include <gint/keyboard.h>
#include <gint/display.h>
#include "util.h"

extern struct Types table_type;

struct Type default_type = {
	.name = "-",
	.id = 0,
	.color = 0xE6D6,
	.buff = {},
	.less = {},
	.null = {}
};

struct Type getTypeFromName(char* name) {
	for(int i = 0; i < NB_TYPES; i++) {
		if(!strcmp(table_type.type[i]->name, name)) return *table_type.type[i];
	}
	return default_type;
}

struct Type getTypeFromId(int id) {
	for(int i = 0; i < NB_TYPES; i++) {
		if(table_type.type[i]->id == id) return *table_type.type[i];
	}
	return default_type;
}

bool parseArray(int array[], char *nom) {
	struct Type type = getTypeFromName(nom);
	for(int i = 0; i < NB_TYPES+1; i++) {
		if(array[i] == 0) return false;
		if(array[i] == type.id) return true;
	}
	return false;
}

float getTypeEffect(struct Type atk, struct Type def) {
	if(parseArray(atk.buff, def.name)) return 2;
	if(parseArray(atk.less, def.name)) return 0.5;
	if(parseArray(atk.null, def.name)) return 0;
	return 1;
}

void drawTypeEffects(struct Type type) {
	dclear(C_WHITE);
	dprint(120,10,C_RED, "Table de %s", type.name);
	dtext(55,40,C_BLUE, "Super efficace");
	dtext(160,40,C_BLUE, "Peu efficace");
	dtext(260,40,C_BLUE, "Non efficace");

	for(int i = 0; i <= 3; i++) {
		for(int j = 0; j <= NB_TYPES; j++) {
			if(i==0) dprint(70+100*i, 60+20*j, C_BLACK, "%s", getTypeFromId(type.buff[j]).name);
			if(i==1) dprint(70+100*i, 60+20*j, C_BLACK, "%s", getTypeFromId(type.less[j]).name);
			if(i==2) dprint(70+100*i, 60+20*j, C_BLACK, "%s", getTypeFromId(type.null[j]).name);
		}
	}
	dupdate();
	wait_for_input(KEY_SHIFT);
}