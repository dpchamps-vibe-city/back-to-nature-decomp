#include "common.h"
#include "game_types.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl17_record_backdrop.h"

void ovl_17_func_800BAF50(void);

void ovl_17_func_800B92B0(void) {
    s32 i;
    u8 *record;
    u8 *base;
    u8 *work;
    u8 *display;

    ovl_17_func_800BAF50();
    base = gOvl17AnimatedRecordBase;
    work = base - 0x28;
    record = base;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteRects((Recon_ovl_17_func_800BAD9C_A0View *) record,
                            *(s32 *) (work + 0x24));
        record += 0x50;
    }
    display = gOvl17RecordWorkBase;
    Ovl17DrawRecordBackdrop(*(s32 *) (display + 0x24));
}
