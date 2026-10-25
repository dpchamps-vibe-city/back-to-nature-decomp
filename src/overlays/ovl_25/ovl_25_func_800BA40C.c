#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

extern u16 D_800C013C;
extern u16 D_800C024C;
extern u16 D_800C0334;

void ovl_25_func_800BA40C(void) {
    u16 *q;
    u16 *p;
    u16 *base;
    s32 i;
    u16 *base2;

    ClearSpriteSourceFlags(&D_800C024C, 4);
    ClearSpriteSourceFlags((&D_800C024C + 0x48), 4);
    ClearSpriteSourceFlags((&D_800C024C + 0x18), 4);
    ClearSpriteSourceFlags((&D_800C024C + 0x30), 4);
    base = &D_800C024C;
    q = base - 0x1D8;
    p = base - 0x1F0;
    for (i = 5; i >= 0; i--) {
        ClearSpriteSourceFlags(p, 4);
        ClearSpriteSourceFlags(q, 4);
        q += 0x3C;
        p += 0x3C;
    }
    base2 = &D_800C013C;
    p = base2 + 0x18;
    q = base2;
    for (i = 1; i >= 0; i--) {
        ClearSpriteSourceFlags(q, 4);
        ClearSpriteSourceFlags(p, 4);
        p += 0x3C;
        q += 0x3C;
    }
    ClearSpriteSourceFlags(&D_800C0334, 4);
    ClearSpriteSourceFlags((&D_800C0334 + 0x18), 4);
    ClearSpriteSourceFlags((&D_800C0334 + 0x30), 4);
    ClearSpriteSourceFlags((&D_800C0334 + 0x48), 4);
    ClearSpriteSourceFlags((&D_800C0334 + 0x60), 4);
    ClearSpriteSourceFlags((&D_800C0334 + 0x78), 4);
}
