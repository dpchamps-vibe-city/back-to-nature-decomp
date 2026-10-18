#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D5868(s16 arg0);
s32 ovl_11_func_800D5D38(u16 a, u16 b);
void ovl_11_func_800F397C(u16 arg0, u16 arg1);
s32 ovl_11_func_800D812C(s16 arg0, s16 arg1, Vec3 *arg2, s16 arg3);
void ovl_11_func_800CF95C(Ovl11Func800CF95CArg arg0, s32 arg1);
/* All target call sites discard the result of this effect helper. */
void ovl_11_func_800DAC08(u16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800DACD4(s16 arg0, s16 arg1, s32 arg2);

s32 ovl_11_func_800D876C(s16 arg0, s16 arg1, s16 arg2) {
    Ovl11Func800CF95CArg sp18;
    Ovl11D124Entry *temp_v0;
    u16 temp_v1;
    u8 var_s4;

    temp_v0 = ovl_11_func_800DAF60((s32) arg0, arg1, arg2);
    if (temp_v0 != NULL) {
        var_s4 = 0;
        if (ovl_11_func_800D5868(temp_v0->unk0) == 1) {
            var_s4 = temp_v0->unk4;
        }
        if (ovl_11_func_800D5D38((u16) temp_v0->unk0, (u16) var_s4) == 1) {
            if ((u16) temp_v0->unk0 == 0x36) {
                ovl_11_func_800F397C(1U, 0U);
                ovl_11_func_800DAC08((u16) temp_v0->unk0, arg0, arg1, arg2);
                ovl_11_func_800DACD4(arg2, arg1, arg0);
            } else {
                ovl_11_func_800DAC08((u16) temp_v0->unk0, arg0, arg1, arg2);
                temp_v0->unk0 = temp_v0->unk2;
                temp_v0->unk4 = 0;
                temp_v0->unk5 = 0;
                temp_v0->unk6 = 0;
            }
            temp_v1 = (u16) temp_v0->unk0;
            if ((temp_v1 == 0x36) || (temp_v1 == 0x40)) {
                ovl_11_func_800D812C(arg1, arg2, (Vec3 *)&sp18, arg0);
                sp18.unk4 = 0;
                ovl_11_func_800CF95C(sp18, 1);
            }
            return 1;
        }
    }
    return 0;
}
