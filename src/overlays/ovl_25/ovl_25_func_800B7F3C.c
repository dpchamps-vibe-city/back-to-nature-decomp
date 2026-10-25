#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_vram_animation_slots.h"

void ovl_25_func_800BAC9C(void);
void ovl_25_func_800BAF20(void);
void ovl_25_func_800BAFCC(void);
void ovl_25_func_800BB2B8(void);
void ovl_25_func_800BB46C(void);
void ovl_25_func_800B82B4(void);
void ovl_25_func_800BAA5C(void);

void ovl_25_func_800B7F3C(void) {
    char *far_base;
    s32 *base;

    switch (*(u8 *)D_800BCD20) {
    case 0:
        ovl_25_func_800BAC9C();
        (*(u8 *)D_800BCD20)++;
        break;
    case 1:
        ovl_25_func_800BAF20();
        (*(u8 *)D_800BCD20)++;
        break;
    case 2:
        ovl_25_func_800BAFCC();
        (*(u8 *)D_800BCD20)++;
        break;
    case 3:
        ovl_25_func_800BB2B8();
        (*(u8 *)D_800BCD20)++;
        break;
    case 4:
        ovl_25_func_800BB46C();
        (*(u8 *)D_800BCD20)++;
        break;
    case 5:
        if (IsSoundTransferComplete() != 0) {
            *(u8 *)D_800BCD20 = 6;
            OpenSoundSequencesAndStart();
            PlaySoundSequenceWithReverb(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)D_800BCD20 == 6) {
        Ovl25InitVramAnimationSlots();
        far_base = (char *)&D_8007AFF0;
        *(Ovl25SetupStatePair *)(far_base + 0x25474) =
            *(Ovl25SetupStatePair *)(far_base + 0x2479C);
        Ovl25RequestVramAnimations((s32)D_8009F78C);
        ovl_25_func_800B82B4();
        ovl_25_func_800BAA5C();
        ResetMessageWindow();
        BeginScreenFadeDecrease(0xA, 0, 2);
        BeginMasterVolumeFadeIn(0xA);
        *(u8 *)D_800BCD20 = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}
