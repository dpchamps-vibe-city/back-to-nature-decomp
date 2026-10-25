#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "random.h"

s32 ovl_23_func_800BA1E0(Ovl23Func800BA1E0Arg *arg0);
/* The original call promotes direction unchanged and narrows only distance. */
s32 ovl_23_func_800B9E04();

s32 ovl_23_func_800B9CCC(Ovl23Func800BA1E0Arg *arg0) {
    s32 x;
    s32 dx;
    s32 dy;
    s32 direction;
    Ovl23D87CView210 *state;

    x = arg0->unk14 / 4096;
    state = (Ovl23D87CView210 *)Ovl23_D_800BF87C_draw;
    dx = x - state->unk210[arg0->unk0].unk0 / 4096;
    dy = arg0->unk18 / 4096 - ((s16 *)&D_800BBA40)[arg0->unk0];
    SquareRoot0(dx * dx + dy * dy);
    direction = ovl_23_func_800BA1E0(arg0);
    if (ovl_23_func_800B9E04(arg0, direction, (s16)(*(s16 *)((u8 *)state + 0x39E) * 3 / 2)) != 1) {
        direction = (s16)(direction + 2 + Rand(2) * 2);
        direction = (s16)(direction % 8);
    }
    return direction;
}
