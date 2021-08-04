#include <gint/display.h>
#include <gint/defs/util.h>
#include "animation.h"
#include "engine.h"

struct sheet
{
    bopti_image_t *img;
    int frame_w;
    int frame_h;
};

extern bopti_image_t img_spritesheet;
struct sheet const anim_player = {
    .img = &img_spritesheet,
    .frame_w = 16,
    .frame_h = 21,
};

static struct anim_frame anim_frame(struct sheet const *sheet, int col, int row) {
    struct anim_frame f = {
        .source = sheet->img,
        .left   = sheet->frame_w * col,
        .top    = sheet->frame_h * row,
        .w      = sheet->frame_w,
        .h      = sheet->frame_h,
    };
    return f;
}

void dframe(int x, int y, struct anim_frame const frame) {
    dsubimage(x, y, frame.source, frame.left, frame.top, frame.w, frame.h,
        DIMAGE_NONE);
}

int anim_player_idle(struct anim_data *data, int init) {
    if(init) {
        data->function = anim_player_idle;
        data->frame = 0;
        data->duration = 100;
    } else {
        data->frame = (data->frame + 1) % 2;
        data->duration += 100;
    }

    data->img = anim_frame(&anim_player, data->dir, data->frame);
    data->dx = 0;
    data->dy = 0;
    return 0;
}