#pragma once

struct Vec2 {
	int x;
	int y;
};

struct Vec2f {
	float x;
	float y;
};

#define VEC2(x, y)                                                             \
	(struct Vec2) { x, y }
#define VEC2F(x, y)                                                            \
	(struct Vec2f) { x, y }
#define VEC2Z  VEC2(0, 0)
#define VEC2FZ VEC2F(0.0, 0.0)

struct Vec2 vec2_add(struct Vec2, struct Vec2);
struct Vec2f vec2f_add(struct Vec2f, struct Vec2f);
struct Vec2 vec2_sub(struct Vec2, struct Vec2);
struct Vec2 vec2_mul(struct Vec2, int scale);
struct Vec2 vec2f_vec2(struct Vec2f);
struct Vec2f vec2_vec2f(struct Vec2);
