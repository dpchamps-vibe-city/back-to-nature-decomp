#include "common.h"
#include "sound_commands.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80051FA8[];
extern u8 D_80053114[];
extern s8 gOvl15MenuState;
extern s16 D_8013759C;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012F6C0(void) {
    s16 temp_s0;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_v0;

    temp_s0 = D_8013759C;
    switch (temp_s0) {
    case 0:
        temp_a1 = gActiveRenderContext->field_D8;
        func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053114);
        if (*(s32 *)((u8 *)gPad0Input + 8) & 0x40) {
            PlaySoundEffect(0);
            D_8013759C = 1;
        }
        break;
    case 1:
        temp_a1_2 = gActiveRenderContext->field_D8;
        func_8001AC10((void *)(temp_a1_2 + 0x18), temp_a1_2 + 0x14, gTextBankOffsets[0] + (s32)D_80051FA8);
        temp_v0 = Ovl15UpdateConfirmationMenu();
        if (temp_v0 != 0) {
            if (temp_v0 <= 0 ? temp_v0 == -1 : temp_v0 == temp_s0) {
                gOvl15MenuState = 0;
            }
        } else {
            gOvl15MenuState = 0xD;
        }
        break;
    }
}
