#pragma once
#include <gint/display.h>

struct anim_data;

typedef int (anim_function_t)(struct anim_data *data, int init);
anim_function_t anim_player_idle;

struct anim_frame
{
   bopti_image_t *source;
   int left, top;
   int w, h;
};

struct anim_data
{
    anim_function_t *function;
    struct anim_frame img;
    int dx, dy;
    int dir;
    int frame;
    int duration;
};

void dframe(int x, int y, struct anim_frame const frame);
int anim_player_idle(struct anim_data *data, int init);