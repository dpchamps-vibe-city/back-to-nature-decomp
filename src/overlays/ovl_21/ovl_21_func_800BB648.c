#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

u8 *func_80014CBC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4, s32 arg5);
void func_8001719C(u8 *arg0);
/* The initializer reads only out and header; its trailing parameters are unused. */
void func_80015704();

void ovl_21_func_800BB648(void) {
    SpriteDataHeader *header;
    SpriteSourceData *sprite;
    u8 *record;
    s32 i;
    s32 j;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    func_80014CBC(0x16, 0, 0x16188, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (func_80014CBC(0x16, 0, 0x16188, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x65CC));
    memcpy(D_800BE108, (u8 *)(D_8005E3B0 + 0x4290), 0x233C);
    header = (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[0]);
    func_80015704(&D_800C0B2C, header);
    func_80015704(&D_800C0B2C - 1, (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[1]));
    for (i = 0; i < 13; i++) {
        func_80015704(&D_800C0B2C + i + 1, (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[i + 2]));
    }
    for (i = 0; i < 6; i++) {
        record = D_800C0458 + i * 0x108;
        for (j = 0; j < 3; j++) {
            sprite = (SpriteSourceData *)(record + j * 0x4C + 0x40);
            func_80015704(sprite, header);
        }
    }
}
