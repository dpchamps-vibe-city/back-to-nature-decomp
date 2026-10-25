#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"

void ovl_23_func_800BB214 (void);
void ovl_23_func_800BB488 (void);
void ovl_23_func_800BB534 (void);
void ovl_23_func_800BB758 (void);
void ovl_23_func_800B81AC (void);
void func_800226F0 (void);

void ovl_23_func_800B7F00(void) {
    s32 *base;

    switch (*(u8 *)&D_800BBA3C) {
    case 0:
        ovl_23_func_800BB214();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 1:
        ovl_23_func_800BB488();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 2:
        ovl_23_func_800BB534();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 3:
        ovl_23_func_800BB758();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 4:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)&D_800BBA3C = 5;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)&D_800BBA3C == 5) {
        ovl_23_func_800B81AC();
        func_800226F0();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)&D_800BBA3C = 0;
        base = (s32 *)&D_8006C838;
        ++base[0x448C >> 2];
    }
}
