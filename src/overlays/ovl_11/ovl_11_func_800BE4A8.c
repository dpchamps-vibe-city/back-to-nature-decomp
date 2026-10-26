#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800BE4A8(Ovl11CollisionQuery *arg0) {
    s32 *position;
    s32 x;
    s32 z;
    s32 y;
    s16 lower;
    s16 upper;
    s16 center;
    Ovl11PlayerCollisionView *player;

    player = (Ovl11PlayerCollisionView *) &gPrimaryObjectBase;
    if (player->region != arg0->object->field_2) {
        return 0;
    }
    position = arg0->position;
    x = position[0];
    center = player->x;
    lower = center - 0xDA;
    upper = center + 0xDA;
    if ((lower < x) && (x < upper)) {
        z = position[2];
        center = player->z;
        lower = center - 0xDA;
        upper = center + 0xDA;
        if ((lower < z) && (z < upper)) {
            y = position[1];
            center = player->y;
            lower = center - 0x14;
            upper = center + 0x14;
            if ((lower < y) && (y < upper)) {
                *arg0->flags = arg0->field_18;
                *arg0->tile = &gPrimaryObjectBase;
                return 1;
            }
        }
    }
    return 0;
}
