#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "beveled_panel.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017A38 (s16 arg0, s16 arg1);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800248B0 (s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800FF27C (s16 arg0, s16 arg1);
void SetVal8005E2BC (s32 arg0);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);

void ovl_11_func_800FEFEC(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 var_s0;
    s32 remainder;

    func_80022580((u32 *) (gActiveRenderContext->field_D8 + 0x58), 1, 0x14, 0x37, 0x118, 0x69);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) ((void *) ((u8 *) &D_80053B22 + *gTextBankOffsets)), 0x20, 0x3B);
    func_80017A38(0, 0);
    if ((D_801273B0 == 0x10) && (IsMessageWindowStatusFinal() == 1)) {
        if ((*(s32 *) ((u8 *) gPad0Input + 0)) & 0xA000) {
            D_801273B6 = (D_801273B6 + 1) % 2;
            PlaySoundEffect(5);
        }
        var_s0 = 6;
        if (D_801273B6 == 0) {
            var_s0 = 5;
            if (D_801273B8_selection[0] == 5) {
                D_801273B8_selection[0] = (u16) D_801273B8_selection[0] - 1;
            }
        }
        temp_v0 = *(s32 *) ((u8 *) gPad0Input + 0);
        if (temp_v0 & 0x5000) {
            s16 *selection = &D_801273B8_selection[0];
            *selection++ = D_801273B8_selection[0]
                + ((temp_v0 & 0x4000) ? 1 : var_s0 - 1);
            PlaySoundEffect(5);
        }
        remainder = D_801273B8_selection[0] % var_s0;
        temp_a0 = *(s32 *) ((u8 *) gPad0Input + 8);
        D_801273B8_selection[0] = remainder;
        if (temp_a0 & 0x20) {
            PlaySoundEffect(1);
            CloseMessageWindow();
            D_801273B0 = 1;
        } else if (temp_a0 & 0x40) {
            PlaySoundEffect(0);
            if (D_801273B0 == 0x10) {
                CloseMessageWindow();
                D_801273B0 = 0x11;
            } else {
                D_801273B0 = 0x10;
            }
        }
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, (s16) ((D_801273B6 * 0x96) + 0x1C), (s16) (((D_801273B8_selection[0] + 1) * 0xE) + 0x41));
    ovl_11_func_800FF27C(D_801273B6, D_801273B8_selection[0]);
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
