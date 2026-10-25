#include "common.h"
#include "pad_actuator_gate.h"
#include "pad_actuator_sequences.h"
#include "sound_commands.h"
#include "message_window.h"

void func_80022580(u32 *, s32, s32, s32, s16, s16);
void func_80017A38(s16, s16);
void func_80017B3C(s32, s32, s32, s32);
void func_800248B0(s32, s16, s16);
void func_80017D9C(s32 *, u16 *, s16, s16, s32);
void ovl_11_func_800FF8C8(s16, s16);

void ovl_11_func_800FF324(void) {
    s16 count;
    s32 buttons;
    s32 remainder;
    s32 *text;

    func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0x58), 1, 0x3A, 0x1E, 0xCA, 0x84);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)(D_80053CB8 + gTextBankOffsets[0]), 0x3E, 0x22);
    func_80017A38(0, 0);
    if (D_801273B0 == 0x20 && IsMessageWindowStatusFinal() == 1) {
        if (((u32 *)gPad0Input)[0] & 0x5000) {
            if (D_801273BA >= 2 && D_801273BC == 0) {
                D_801273BA = 1;
            }
            D_801273BC += (((u32 *)gPad0Input)[0] & 0x4000) ? 1 : 3;
            PlaySoundEffect(5);
        }
        D_801273BC %= 4;
        remainder = 2;
        if (D_801273BC == 0) {
            remainder = 3;
        }
        count = remainder;
        buttons = ((u32 *)gPad0Input)[0];
        if (buttons & 0xA000) {
            s16 *selection = &D_801273BA;
            *selection++ = D_801273BA + ((buttons & 0x2000) ? 1 : count - 1);
            PlaySoundEffect(5);
        }
        remainder = D_801273BA % count;
        buttons = ((u32 *)gPad0Input)[2];
        D_801273BA = remainder;
        if (buttons & 0x20) {
            CloseMessageWindow();
            PlaySoundEffect(1);
            D_801273B0 = 1;
        } else if (buttons & 0x40) {
            switch (D_801273BC) {
            case 0:
                D_8012CEE8_options[0] = remainder;
                break;
            case 1:
                if (remainder == 0 && D_8012CEE8_options[1] == 0) {
                    SetPadActuatorsEnabled(1);
                    RequestPadActuatorSequence(0, 0);
                }
                D_8012CEE8_options[1] = D_801273BA != 1;
                break;
            case 2:
                D_801273C8[remainder]();
                D_8012CEE8_options[2] = D_801273BA != 1;
                break;
            case 3:
                D_801273D0[remainder]();
                D_8012CEE8_options[3] = D_801273BA != 1;
                break;
            }
            PlaySoundEffect(0);
        } else if (buttons & 0x10) {
            PlaySoundEffect(0);
            if (D_801273B0 == 0x20) {
                CloseMessageWindow();
                D_801273B0 = 0x21;
            } else {
                D_801273B0 = 0x20;
            }
        }
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, (s16)(D_801273BA * 65 + 0x42), (s16)((D_801273BC + 1) * 28 + 0x28));
    text = (s32 *)(gActiveRenderContext->field_D8 + 0x54);
    func_80017D9C(text, (u16 *)(D_80053D6A + gTextBankOffsets[0]), 0x46, 0x3E, D_8012CEE8_options[0] != 0);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0xE + gTextBankOffsets[0]), 0x87, 0x3E, D_8012CEE8_options[0] != 1);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x1C + gTextBankOffsets[0]), 0xC8, 0x3E, D_8012CEE8_options[0] != 2);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x38 + gTextBankOffsets[0]), 0x46, 0x5A, D_8012CEE8_options[1] != 1);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x3E + gTextBankOffsets[0]), 0x87, 0x5A, D_8012CEE8_options[1] != 0);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x46 + gTextBankOffsets[0]), 0x46, 0x76, D_8012CEE8_options[2] != 1);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x54 + gTextBankOffsets[0]), 0x87, 0x76, D_8012CEE8_options[2] != 0);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x5E + gTextBankOffsets[0]), 0x46, 0x92, D_8012CEE8_options[3] != 1);
    func_80017D9C(text, (u16 *)(D_80053D6A + 0x66 + gTextBankOffsets[0]), 0x87, 0x92, D_8012CEE8_options[3] != 0);
    ovl_11_func_800FF8C8(D_801273BA, D_801273BC);
}
