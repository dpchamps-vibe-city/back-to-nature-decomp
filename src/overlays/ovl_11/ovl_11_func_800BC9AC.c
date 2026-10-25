#include "common.h"
#include "game_types.h"
#include "sound_driver.h"
#include "sound_commands.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[4];
    u16 unk8;
} Ovl11BC9ACView;

typedef struct {
    char pad_000[0xE4C8];
    s16 field_E4C8;
} Ovl11BC9ACBigView;

typedef struct {
    char pad_000[0x25388];
    Ovl11BC9ACView *ptr;
    char pad_0002[0x2549C - 0x2538C];
    s16 field_2549C;
} Ovl11BC9ACFarView;

s32 ovl_11_func_800C0834(s16 arg0);

void ovl_11_func_800BC9AC(void) {
    s16 temp_v1;
    s32 temp_a2;
    s32 var_s1;

    if (D_801217A4 != 0) {
        StopSoundSequence(0);
    }
    var_s1 = 0x64;
    temp_a2 = ovl_11_func_800C0834(((struct struct_8006C838_time *) D_8006C838)->field_44C0);
    if (((Ovl11BC9ACFarView *) &D_8007AFF0)->ptr->unk8 & 0x1000) {
        var_s1 = 0x50;
    }
    temp_v1 = ((Ovl11BC9ACFarView *) &D_8007AFF0)->ptr->unk0;
    if (temp_v1 == 0x37) {
        var_s1 = 0x50;
    }
    if (temp_v1 == 0x43) {
        var_s1 = 0x50;
    }
    if (temp_a2 == 3) {
        if ((temp_v1 != 0xB) && (temp_v1 != 0x33)) {
            var_s1 /= 2;
        }
        if ((((Ovl11BC9ACBigView *) D_8006C838)->field_E4C8 == 1) && !(((Ovl11BC9ACFarView *) &D_8007AFF0)->ptr->unk8 & 0x1000)) {
            var_s1 = 0x64;
        }
        if (((Ovl11BC9ACFarView *) &D_8007AFF0)->ptr->unk2 == 0xF) {
            var_s1 = 0;
        }
        if (((struct struct_8006C838_button *) D_8006C838)->field_44BA == 3) {
            var_s1 = 0;
        }
    }
    if (((Ovl11BC9ACFarView *) &D_8007AFF0)->ptr->unk2 == 0x28) {
        var_s1 = 0x64;
    }
    if (D_801217A4 != 0) {
        PlaySoundSequenceWithReverb(0, 0, ((Ovl11BC9ACFarView *) &D_8007AFF0)->field_2549C, var_s1);
        return;
    }
    SetSoundSequenceSlotVolume(0, 0, 0, var_s1, 0xA);
}
