#include "common.h"
#include "game_types.h"

void func_800158E4(SpriteSourceData *src);

void func_80015F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);

void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3) {
    func_80015F80(arg0, (s32) arg1, (s32) arg1->field_4, (s32) arg1->field_5, (s16) (s32) arg2, (s16) (s32) arg3, arg1->field_8, 0, 0U);
    func_800158E4(arg1);
}
