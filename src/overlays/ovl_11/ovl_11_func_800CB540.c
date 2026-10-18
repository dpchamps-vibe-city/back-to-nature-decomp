#include "common.h"
#include "game_types.h"

void ovl_11_func_800D7328(s16 *arg0);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
s32 ovl_11_func_800D6A94(s16 arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800CB540(u16 *arg0) {
    Ovl11C7010Object *obj = (Ovl11C7010Object *) &D_80071A00;
    s32 *temp_s2;
    s32 *temp_s3;
    s32 var_s1;
    u16 var_s5;

    temp_s2 = (s32 *) ((u8 *) obj + 0xF0);
    ovl_11_func_800D7328((s16 *) temp_s2);
    var_s5 = 6;
    temp_s3 = (s32 *) ((u8 *) obj - 0x51C8);
    var_s1 = 0xB;
    if ((*(ItemData **) ((u8 *) temp_s3 + 0x20))[(s16) *arg0].u0.flags & 0x100) {
        var_s5 = 0xE;
        var_s1 = 0x25;
    }
    ovl_11_func_800D7348((u16 *) temp_s2, arg0);
    ovl_11_func_800D6A94(*(s16 *) ((u8 *) obj + 0xF0));
    ovl_11_func_800C70CC(obj, var_s5, var_s1 + (*(u16 *) ((u8 *) temp_s3 + 0x5200)), -2);
    obj->unk6C |= 0x8004;
    return 0;
}
