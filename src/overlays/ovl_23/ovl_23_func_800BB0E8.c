#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_23_func_800BB0E8(void) {
    u8 *p;
    u8 *q;
    u32 base;
    s32 i;

    base = (u32)D_800BFA90;
    q = (u8 *)(base - 0x1D4);
    p = (u8 *)base;
    for (i = 5; i >= 0; i--) {
        AddSpriteSourceFlags((u16 *)p, 4);
        AddSpriteSourceFlags((u16 *)q, 4);
        q += 0x50;
        p += 0x44;
    }
}
