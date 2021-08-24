#include "vec2.h"

struct Vec2
vec2_add(struct Vec2 v1, struct Vec2 v2)
{
	return VEC2(v1.x + v2.x, v1.y + v2.y);
}

struct Vec2f
vec2f_add(struct Vec2f v1, struct Vec2f v2)
{
	return VEC2F(v1.x + v2.x, v1.y + v2.y);
}

struct Vec2
vec2_sub(struct Vec2 v1, struct Vec2 v2)
{
	return VEC2(v1.x - v2.x, v1.y - v2.y);
}

struct Vec2
vec2_mul(struct Vec2 v, int scale)
{
	return VEC2(v.x * scale, v.y * scale);
}

struct Vec2f
vec2f_mul(struct Vec2f v, int scale)
{
	return VEC2F(v.x * scale, v.y * scale);
}

struct Vec2
vec2f_vec2(struct Vec2f v)
{
	return VEC2(v.x, v.y);
}

struct Vec2f
vec2_vec2f(struct Vec2 v)
{
	return VEC2F(v.x, v.y);
}

struct Vec2f
vec2f_lerp(struct Vec2f from, struct Vec2f to, float scale) {
	/* Linear interpolation: can be used for camera and animations.
	 * 'scale' is the transformation speed, 0 slow and 1 fast. */
	return VEC2F(
		from.x * (1.0-scale) + to.x * scale,
		from.y * (1.0-scale) + to.y * scale
	);
}
