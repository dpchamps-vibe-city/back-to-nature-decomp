#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl21_scene_sprite_draw.h"


void ovl_21_func_800BA5D0(void) {
    u8 *base;
    s32 *tbl;
    s32 *p;
    s32 *q;
    s32 i;
    s32 next;
    s32 j;
    s32 ofs;

    i = 0;
    base = (u8 *)D_800C0448;
    tbl = (s32 *)(base + 0x34);
    while (i < 6) {
        next = i + 1;
        ofs = ((i << 5) + i) << 3;
        p = (s32 *)(ofs + (s32)tbl);
        q = p;
        for (j = 2; j >= 0; j--) {
            if (*q != 0) {
                Ovl21DrawSlotSprite(p);
            }
            p = (s32 *)((u8 *)p + 0x4C);
            q = (s32 *)((u8 *)q + 0x4C);
        }
        i = next;
    }
}
