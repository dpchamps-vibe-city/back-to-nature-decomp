#include "common.h"
#include "sound_commands.h"
void ovl_25_func_800BA2E8 (void);
void ovl_25_func_800BA40C (void);
void ovl_25_func_800B93BC (void);
void ovl_25_func_800B8848 (void);
void ovl_25_func_800B8CEC (void);
void ovl_25_func_800B8D70 (void);
void ovl_25_func_800B93E4 (void);
void ovl_25_func_800B9208 (void);
void ovl_25_func_800B8A30(void);

void ovl_25_func_800B8728(void) {
    s32 temp_v0;
    s32 q;
    s32 decrement;

    if ((*(s32 *) ((u8 *) gPad0Input + 8)) & 0x800) {
        temp_v0 = D_800BFE44_view.unk4 != 1;
        D_800BFE44_view.unk4 = temp_v0;
        if (temp_v0 == 1) {
            ovl_25_func_800BA2E8();
            D_800BFE44_view.unk4C8 = PlaySoundEffect(0x36);
        } else {
            ovl_25_func_800BA40C();
        }
    }
    if (D_800BFE44_view.unk4 == 1) {
        ovl_25_func_800B93BC();
        return;
    }
    ovl_25_func_800B8848();
    ovl_25_func_800B8CEC();
    ovl_25_func_800B8A30();
    ovl_25_func_800B8D70();
    ovl_25_func_800B93E4();
    ovl_25_func_800B9208();
    q = (D_800BFE44_view.unkC + 1) / 10000;
    decrement = q * 10000 - 1;
    D_800BFE44_view.unkC = (u16) D_800BFE44_view.unkC - decrement;
}
