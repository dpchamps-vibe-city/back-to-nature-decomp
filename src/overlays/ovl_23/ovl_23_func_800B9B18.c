#include "common.h"
#include "game_types.h"

s32 ovl_23_func_800B9E04(Ovl23Func800BA1E0Arg *arg0, s16 arg1, s16 arg2);

void ovl_23_func_800B9B18(Ovl23Func800BA1E0Arg *arg0) {
    s16 state;
    s16 direction;
    s32 x;
    s32 diagonal;
    s32 limit;
    s32 dy;
    s32 dx;
    s32 speed;

    state = *(s16 *)((u8 *)arg0 + 4);
    switch (state) {
    case 1:
        speed = *(s32 *)((u8 *)arg0 + 0x10);
        break;
    case 3:
        speed = (Ovl23_D_800BF87C_bounds.unk3B2 << 12) / 10;
        break;
    default:
        return;
    }
    diagonal = (speed * 1000) / 1414;
    if (ovl_23_func_800B9E04(arg0, *(s16 *)((u8 *)arg0 + 6),
            Ovl23_D_800BF87C_bounds.unk39E) == 1) {
        dx = 0;
        direction = *(s16 *)((u8 *)arg0 + 6);
        dy = 0;
        switch (direction) {
        case 0:
            dy = speed;
            break;
        case 1:
            dy = diagonal;
            dx = dy;
            break;
        case 2:
            dx = speed;
            break;
        case 3:
            dy = -diagonal;
            dx = diagonal;
            break;
        case 4:
            dy = -speed;
            break;
        case 5:
            dy = -diagonal;
            dx = dy;
            break;
        case 6:
            dx = -speed;
            break;
        case 7:
            dy = diagonal;
            dx = -dy;
            break;
        }
        x = arg0->unk14 + dx;
        arg0->unk14 = x;
        arg0->unk18 += dy;
        limit = (Ovl23_D_800BF87C_bounds.unk39C + 0x37) << 12;
        if (x >= limit) {
            arg0->unk14 = limit;
        }
        if (arg0->unk14 <= -0x30000) {
            arg0->unk14 = -0x30000;
        }
    }
}
