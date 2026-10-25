#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"
#include "sprite_render.h"

extern s16 D_8012DB18;
extern SVECTOR D_8012DB20;
extern s32 D_8012DB28[4];

s32 func_8001DFD4 (s32 *arg0, SVECTOR *arg1);

void ovl_11_func_80120850(s32 arg0) {
    s32 temp_v1;
    s32 var_v0_2;
    s32 temp_v0;
    s32 var_s1;
    s32 var_v0;

    if (arg0 == 0) {
        var_v0 = csin(D_8012DB18);
        if (var_v0 < 0) {
            var_v0 += 0x3FF;
        }
        var_s1 = (var_v0 << 6) >> 0x10;
        temp_v1 = (u16) D_8012DB18 + 0x16;
        var_v0_2 = (s16) temp_v1;
        D_8012DB18 = temp_v1;
        if (var_v0_2 < 0) {
            var_v0_2 += 0xFFF;
        }
        D_8012DB18 = temp_v1 - ((var_v0_2 >> 0xC) << 0xC);
    } else {
        var_s1 = 0;
    }
    D_8012DB20.vx = D_8012DB78.x + 0x96;
    D_8012DB20.vy = D_8012DB78.y - 0x64;
    D_8012DB20.vz = D_8012DB78.z - 0x96;
    temp_v0 = func_8001DFD4(&D_8012DB28[0], &D_8012DB20);
    if (temp_v0 != 0) {
        if (arg0 != 0) {
            D_8012DB88 += 1;
        }
        DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((temp_v0 >> 2) * 4), (s32) &gSharedSpriteSource, (s32) (u8) D_8012DB84, 0, (s16) (*(s16 *) ((u8 *) D_8012DB28 + 0)), (s16) (var_s1 + ((*(u16 *) ((u8 *) D_8012DB28 + 4)) + ((u16) D_8012DB88 & 3))));
    }
}
