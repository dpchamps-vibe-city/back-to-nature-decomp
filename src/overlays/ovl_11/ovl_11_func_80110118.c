#include "common.h"
#include "sound_commands.h"
#include "game_flags.h"

typedef struct {
    /* 0x00 */ char pad_00[0x30];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0xB6 - 0x32];
    /* 0xB6 */ u16 unkB6;
    /* 0xB8 */ char pad_B8[0xDC - 0xB8];
    /* 0xDC */ s32 unkDC;
} Struct_80110118;

s32 ovl_11_func_800F3BCC(u16 arg0);

u16 ovl_11_func_80110118(Struct_80110118 *arg0) {
    char *far_base;

    ovl_11_func_800F3BCC(arg0->unkB6);
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        PlaySoundEffect(0x13);
        SetGameFlag(7, 1);
    }
    arg0->unkB6 = 0;
    arg0->unkDC = 0;
    return 1;
}
