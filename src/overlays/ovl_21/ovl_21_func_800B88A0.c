#include "common.h"
#include "psyq/libspu.h"
#include "sound_commands.h"
#include "message_window.h"

void ovl_21_func_800BA4C0(void);
void ovl_21_func_800B9798(s16 arg0, s16 arg1);
void SetVal8005E2BC(s32 arg0);

void ovl_21_func_800B88A0(void) {
    u8 *base;
    u8 *p;
    u8 *q;
    u8 *r;

    ovl_21_func_800BA4C0();
    base = (u8 *)D_800C0448;
    if (*(s16 *)(base + 0x988) == -1) {
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            *(s16 *)(base + 0x988) = 0;
            D_800C0448[0] = 5;
            return;
        }
    }
    p = (u8 *)D_800C0448;
    if (*(s16 *)(p + 0x988) >= 2) {
        s32 status;

        *(s16 *)(p + 0x988) = *(u16 *)(p + 0x988) + 1;
        status = SpuGetKeyStatus(D_800BCC60[*(s16 *)(p + 0x986)]);
        if (status == 0 || *(s16 *)(p + 0x988) >= 0x96) {
            ovl_21_func_800B9798(1, 0);
            SetVal8005E2BC(1);
            SetMessagePortraitEnabled(1);
            *(s16 *)(p + 0x988) = -1;
            return;
        }
    }
    q = (u8 *)D_800C0448;
    if (*(s16 *)(q + 0x988) == 1) {
        if (GetMessageWindowStatus() == 5) {
            *(s16 *)(q + 0x986) = PlaySoundEffect(0x45);
            ovl_21_func_800B9798(1, 0xB);
            *(s16 *)(q + 0x988) = 2;
        }
    }
    r = (u8 *)D_800C0448;
    if (*(s16 *)(r + 0x988) == 0) {
        CloseMessageWindow();
        SetVal8005E2BC(0);
        SetMessagePortraitEnabled(0);
        RequestMessageWindow(4, D_800BCCCC[*(s16 *)(r + 0x642)]);
        *(s16 *)(r + 0x988) = 1;
    }
}
