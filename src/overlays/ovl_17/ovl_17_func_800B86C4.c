#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl17_record_sprite_animation.h"
#include "ovl17_record_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl17_record_backdrop.h"

void func_80017A08(s32 arg0, s32 arg1);

void ovl_17_func_800B86C4(void) {
    s32 i = 0;
    u8 *base;
    s32 data = (s32)D_800556A8;
    u8 *work;
    u8 *state;

    do {
        base = gOvl17RecordWorkBase;
        func_80017A08((u16)i, *(u16 *)(base + i * 0x50 + 0x40) + data);
        i++;
    } while (i < 6);
    Ovl17SelectAllRecordAnimations2();
    work = gOvl17AnimatedRecordBase;
    for (i = 0; i < 6; i++) {
        Ovl17DrawRecordSpriteQuads((Recon_ovl_17_func_800BAD9C_A0View *)work, 0x40000);
        work += 0x50;
    }
    Ovl17DrawRecordBackdrop(0x40000);
    RequestMessageWindow(4, 4);
    if (GetMessageWindowStatus() == 2) {
        state = gOvl17RecordWorkBase;
        *(s16 *)(state + 0x2CE) = 0;
        state[4] = i;
    }
}
