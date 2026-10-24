#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

s32 GetVal8005E3B0(void);

s32 func_8001205C(void);

s32 func_8001AF44(u32 arg0);

s32 ovl_11_func_800E39B8(s32 *arg0, u32 arg1);

s32 ovl_11_func_800E351C(u8 *arg0, s16 arg1, s16 arg2) {
    Ovl11Rect801248B4 rect;
    u8 *acc;
    u8 *end;
    s32 i;
    s32 diff;
    s32 size;
    s32 *p;

    acc = arg0;
    for (i = 0; i < 0x32; i++) {
        diff = _D_801247E8[i + 1] - _D_801247E8[i];
        D_80129420[i] = ovl_11_func_800E39B8((s32 *)acc, diff);
        acc += diff;
    }

    acc = arg0;
    rect = D_801248B4;
    rect.x += arg1;
    rect.y += arg2;
    LoadImage((RECT *)&rect, (u_long *)acc);
    acc += D_801248B4.w * D_801248B4.h * 2;

    if (func_8001AF44(2) == 1) {
        DrawSync(0);
        return 0;
    }

    end = arg0 + _D_801247E8[50] - _D_801247E8[0];
    diff = end - acc;
    if (func_8001205C() >= diff) {
        p = &D_801294E8;
        *p = GetVal8005E3B0() - diff;
        size = *p;
        if (size & 3) {
            size -= size & 3;
        }
        *p = size;
        memcpy((void *)size, acc, diff);
    }
    DrawSync(0);
    return 0;
}
