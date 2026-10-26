#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl17_record_work_init.h"

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);

void ovl_17_func_800B861C(void) {
    s32 i;
    u8 *base;
    u8 *record;

    for (i = 0; i < 6; i++) {
        base = gOvl17AnimatedRecordBase + i * 0x50;
        if (i != 2) {
            ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        }
    }
    record = D_800BD910;
    ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
    ovl_17_func_800BA808(0x40000);
    RequestMessageWindow(4, 2);
    if (GetMessageWindowStatus() == 2) {
        Ovl17ResetCardRows();
        record[-0xC4] = 5;
    }
}
