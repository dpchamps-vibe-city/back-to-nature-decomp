#include "common.h"
#include "game_types.h"

u16 func_80015A18(SpriteSourceData *arg0, s32 arg1);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3) {
    s16 temp_v0;
    s16 frame;

    if (*(s16 *)(arg0 + 2) != 0) {
        temp_v0 = func_80015A18((SpriteSourceData *)&D_800A0728, *(s16 *)arg0);
        frame = (s16)(*(s16 *)(arg0 + 2) / 10) % temp_v0;
        func_80015EE8(arg1, (s32)&D_800A0728, arg0[0],
                     (temp_v0 - frame - 1) & 0xFF, arg2, arg3);
    }
}
