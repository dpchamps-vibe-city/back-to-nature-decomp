#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void ovl_19_func_800BB548(s16 *arg0);
s32 func_8001DFD4(s32 *arg0, SVECTOR *arg1);
void func_80015840(ObjectState *obj, u8 arg1);
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3);
void func_800248E8(s32 arg0, s16 arg1, s16 arg2, s16 arg3);

void ovl_19_func_800BAFAC(s32 arg0, ObjectState *arg1, s16 *arg2, s16 arg3) {
    s32 depth;
    s32 offset;

    ovl_19_func_800BB548(arg2);
    D_800BD088[0] = arg2[0];
    D_800BD088[1] = arg2[1];
    D_800BD088[2] = arg2[2];
    depth = func_8001DFD4(D_800BD050, (SVECTOR *) D_800BD088);
    func_80015840(arg1, arg3 & 0xFF);
    offset = (depth >> 3) * 4;
    func_80015BF0(arg0 + offset, (SpriteSourceData *) arg1,
                 (s16) D_800BD050[0], (s16) D_800BD050[1]);
    func_800248E8(gActiveRenderContext->field_120 + offset,
                 (s16) D_800BD050[0], (s16) D_800BD050[1], 0);
}
