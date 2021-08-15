#pragma once
#include <gint/display.h>

struct AnimData;

typedef int (anim_function_t)(struct AnimData *data, int init);
anim_function_t anim_player_walking;
anim_function_t anim_player_idle;

struct AnimFrame
{
   /*the image*/
   bopti_image_t *source;
   /*the left and top pixels to know where to start drawing*/
   int left, top;
   /*the width and height of one frame*/
   int w, h;
};

struct AnimData
{
   /*the function to call to update the frame*/
    anim_function_t *function;
    /*the anim frame*/
    struct AnimFrame img;
    /*if the animation needs to move*/
    int dx, dy;
    /*the direction*/
    int dir;
    /*the current frame*/
    int frame;
    /*the duration of one frame*/
    int duration;
};

/*draw the frame*/
void dframe(int x, int y, struct AnimFrame const frame);
/*animation for player walking*/
int anim_player_walking(struct AnimData *data, int init);
/*animation for player doing nothing*/
int anim_player_idle(struct AnimData *data, int init);
