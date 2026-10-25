#include "common.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "message_window.h"

void func_80017A38(s16 arg0, s16 arg1);
void ovl_11_func_8011ECC4(void);
void SetVal8005E2BC(s32 arg0);
void ovl_11_func_8011EC54(void);

void ovl_11_func_8011EA0C(void) {
    s32 direction;
    u32 buttons;

    func_80017A38(0, 2);
    direction = 0;
    if (D_80128420 != 7 && IsMessageWindowStatusFinal()) {
        buttons = ((u32 *)gPad0Input)[0] & 0xF000;
        if (buttons) {
            s32 offset;
            u8 *entry;

            switch (buttons) {
            case 0x1000:
                direction = 0;
                break;
            case 0x4000:
                direction = 1;
                break;
            case 0x8000:
                direction = 2;
                break;
            case 0x2000:
                direction = 3;
                break;
            }
            offset = direction + D_80128422 * 8;
            entry = D_801284AC;
            entry -= -offset;
            D_80128422 = entry[4];
            PlaySoundEffect(5);
        }
    }
    ovl_11_func_8011ECC4();
    if (D_80128420 == 7) {
        if (GetMessageWindowStatus() == 2) {
            D_80128420 = 6;
            CloseMessageWindow();
        }
    } else {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        if (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 == 1) {
            RequestMessageWindow(3, 0x3AB);
        } else {
            RequestMessageWindow(3, 0x3B1);
        }
    }
    if (D_80128420 != 7 && IsMessageWindowStatusFinal()) {
        u32 pressed;

        pressed = ((u32 *)gPad0Input)[2];
        if (pressed & 0x120) {
            if (IsMessageWindowStatusFinal()) {
                ovl_11_func_8011EC54();
            }
        } else if (pressed & 0x50) {
            PlaySoundEffect(0);
            D_80128420 = 7;
            CloseMessageWindow();
            RequestMessageWindow(3, D_80128422 + 0x385);
        } else if (pressed & 0x800) {
            if (((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 == 1) {
                PlaySoundEffect(0);
                BeginScreenFadeIncrease(10, 0, 2);
                D_80128420 = 11;
            } else {
                PlaySoundEffect(2);
            }
        }
    }
}
