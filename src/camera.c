#include "vec2.h"
#include "define.h"
#include "camera.h"

struct Camera camera_new(struct Vec2f *target) {
	return (struct Camera) {
		.pos = *target,
		.offset = vec2f_vec2(*target),
		.target = target,
	};
}

void camera_update(struct Camera *c) {
	c->pos = *c->target;
	c->offset = vec2f_vec2(c->pos);
}
