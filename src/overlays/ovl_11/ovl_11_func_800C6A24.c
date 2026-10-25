#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

/* 8001FAD0 calls 800212A8; its result reaches jr ra unchanged. */
s16 ovl_11_func_800CB020(s16 arg0);
/* 800C6AD4 forwards unk3C + 1 without narrowing; 800C70CC stores it with sh. */
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C6A24(Ovl11C7010Object *arg0) {
    s16 temp_s0;
    s32 soundBase;
    s32 soundOffset;

    temp_s0 = (s16) (*(u16 *) ((u8 *) arg0 + 0x18)) / 20;
    soundOffset = ((arg0->unk3C - 0x16) / 2) * 5;
    soundBase = temp_s0 + 0x1D;
    D_80128C70 = PlaySoundEffect((s16) (soundOffset + soundBase));
    ovl_11_func_800C70CC(arg0, arg0->unk3C + 1, ovl_11_func_800CB020((s16) ((u16) arg0->unk84 - ((*(u16 *) ((u8 *) arg0 + 0x86)) - temp_s0))) + (arg0->unk38 * 5), -2);
    arg0->sprite.field_5 = temp_s0 + 1;
}
