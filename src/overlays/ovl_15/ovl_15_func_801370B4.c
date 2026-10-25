#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

s8 ovl_15_func_801370B4(void) {
    s32 temp_v1;
    s32 temp_v1_2;

    func_80017B3C(gActiveRenderContext->field_D8 + 8, (s32) (D_80051D80 + gTextBankOffsets[0]), 0x2B, 0xC4);
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_v1 & 0x40) {
        if (D_80137588 == 0) {
            PlaySoundEffect(0);
        } else {
            PlaySoundEffect(1);
        }
        return D_80137588;
    }
    if (temp_v1 & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    temp_v1_2 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_v1_2 & 0x4000) {
        PlaySoundEffect(5);
        D_80137588 = (u8) D_80137588 + 1;
    } else if (temp_v1_2 & 0x1000) {
        PlaySoundEffect(5);
        D_80137588 = (u8) D_80137588 - 1;
    }
    if (D_80137588 < 0) {
        D_80137588 = 1;
    }
    if (D_80137588 >= 2) {
        D_80137588 = 0;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x23, (D_80137588 * 0xE) + 0xC8);
    return 2;
}
