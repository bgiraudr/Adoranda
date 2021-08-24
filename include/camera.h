#pragma once
#include "vec2.h"

struct Camera {
	/* true position */
	struct Vec2f pos;
	/* used by draw functions as an offset */
	struct Vec2 offset;
	/* target to follow */
	struct Vec2f *target;
};

struct Camera camera_new(struct Vec2f *target);
