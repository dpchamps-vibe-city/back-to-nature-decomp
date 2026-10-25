#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"

s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0) {
    SVECTOR pos;

    pos = *(SVECTOR *) ((u8 *) arg0 + 0x10);
    switch (arg0->unk4) {
    case 0:
        pos.vz -= 200;
        break;
    case 1:
        pos.vx -= 200;
        break;
    case 2:
        pos.vz += 200;
        break;
    case 3:
        pos.vx += 200;
        break;
    }
    Ovl19AdjustPositionToSurface((s16 *) &pos);
    return pos.vy != *(s16 *) ((u8 *) arg0 + 0x12);
}
