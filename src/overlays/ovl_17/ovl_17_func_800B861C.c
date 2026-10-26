#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl17_record_work_init.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl17_record_backdrop.h"

void ovl_17_func_800B861C(void) {
    s32 i;
    u8 *base;
    u8 *record;

    for (i = 0; i < 6; i++) {
        base = gOvl17AnimatedRecordBase + i * 0x50;
        if (i != 2) {
            Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        }
    }
    record = D_800BD910;
    Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 2);
    if (GetMessageWindowStatus() == 2) {
        Ovl17ResetCardRows();
        record[-0xC4] = 5;
    }
}
