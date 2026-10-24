#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void func_80015840(ObjectState *obj, u8 arg1);
void func_8001585C(ObjectState *obj, u8 arg1);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80015F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);

static __inline__ void drawTile(s32 ot, u8 *sprite, s16 x) {
    u8 *work = sprite - 0x22C;
    func_80015EE8(ot, (s32)sprite, work[0x230], work[0x231], x, 0);
}

void ovl_17_func_800BA808(s32 arg0) {
    s16 x;
    s16 edgeX;
    s16 edgeX2;
    s16 edgeX3;
    s32 pixels;
    s32 list;
    s32 ot;
    u8 *edgeBase;
    u8 *endBase;

    ot = D_8005E3C0->field_D8 + 0x58;
    func_80015840((ObjectState *)D_800BDA74, 0);
    func_8001585C((ObjectState *)D_800BDA74, 0);
    for (x = arg0 / 4096 - (arg0 / 262144) * 64 - 64; x < 384; x += 64) {
        drawTile(D_8005E3C0->field_D8 + 0x5C, D_800BDA74, x);
    }
    func_80015840((ObjectState *)D_800BDA74, 1);
    x = (arg0 / 4096) * 2 - (arg0 / 155648) * 76 - 76;
    for (; x < 396; x += 76) {
        drawTile(D_8005E3C0->field_D8 + 0x18, D_800BDA74, x);
    }
    pixels = arg0 / 4096;
    if ((u32)(pixels + 191) < 383) {
        func_80015840((ObjectState *)D_800BDA74, 0);
        func_8001585C((ObjectState *)D_800BDA74, 1);
        edgeX = pixels + 160;
        edgeX -= 32;
        edgeBase = D_800BDA74 - 0x22C;
        func_80015EE8(ot, (s32)D_800BDA74, edgeBase[0x230], edgeBase[0x231], edgeX, 0);
        func_80015840((ObjectState *)D_800BDA74, 2);
        edgeX2 = pixels + 224;
        edgeX2 -= 32;
        func_80015EE8(ot, (s32)D_800BDA74, edgeBase[0x230], edgeBase[0x231], edgeX2, 0);
    }
    if ((u32)(pixels - 1857) < 383) {
        func_80015840((ObjectState *)D_800BDA74, 0);
        func_8001585C((ObjectState *)D_800BDA74, 1);
        edgeX3 = pixels - 1888;
        edgeX3 -= 32;
        endBase = D_800BDA74 - 0x22C;
        func_80015EE8(ot, (s32)D_800BDA74, endBase[0x230], endBase[0x231], edgeX3, 0);
    }
    for (list = 0; D_800BB5F0[list].id != 255; list++) {
        if ((arg0 / 4096) * 2 + HWD0 > D_800BB5F0[list].x / 4096 &&
            (arg0 / 4096) * 2 - HWD0 < D_800BB5F0[list].x / 4096) {
            x = HWD0 / 2 + (arg0 / 4096) * 2 - D_800BB5F0[list].x / 4096;
            func_80015840((ObjectState *)&D_800BDDF8, D_800BB5D4[D_800BB5F0[list].id]);
            func_8001585C((ObjectState *)&D_800BDDF8, x < HWD0 / 2);
            func_80015F80(D_8005E3C0->field_D8 + 0x14, (s32)&D_800BDDF8,
                          D_800BDDF8.field_4, D_800BDDF8.field_5,
                          x, (s16)VWD0, 0x1000, 0, 0);
        }
    }
}
