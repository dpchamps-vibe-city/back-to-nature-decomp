#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl17_record_work_init.h"
#include "ovl17_record_sprite_draw.h"

void ovl_17_func_800BA808(s32 arg0);

void ovl_17_func_800B8534(void) {
    s32 i;
    s32 state;
    s32 result;
    s32 skip;
    u8 *base;
    u8 *record;
    u8 *origin;

    i = 0;
    skip = 2;
    record = gOvl17AnimatedRecordBase;
    origin = record;
    for (; i < 6; i++) {
        if (i == skip) {
            record = origin + 0xF0;
            i = 3;
        }
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
        record += 0x50;
    }
    base = D_800BD910;
    Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
    ovl_17_func_800BA808(0x40000);
    RequestMessageWindow(4, 1);
    state = GetMessageWindowStatus();
    if (state == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            base[-0xC4] = 4;
        } else if (result == state) {
            Ovl17ResetCardRows();
            base[-0xC4] = result;
        }
    }
}
