#pragma once
#include "define.h"
#include <stdint.h>

struct Type {
	char* name;
	const int id;
	int buff[NB_TYPES+1];
	int less[NB_TYPES+1];
	int null[NB_TYPES+1];
	const uint16_t color;
};

struct Types {
    struct Type *type[NB_TYPES];
};

bool parseArray(int array[], char *nom);
float getTypeEffect(struct Type atk, struct Type def);
struct Type getTypeFromName(char* name);
struct Type getTypeFromId(int id);
void drawTypeEffects(struct Type type);