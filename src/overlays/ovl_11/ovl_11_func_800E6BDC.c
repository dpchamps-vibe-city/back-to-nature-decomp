#include "common.h"
#include "sound_driver.h"
#include "sound_commands.h"

s32 ovl_11_func_800E6BDC(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    char *far_base;
    char *far_base2;
    s16 var_s0;

    if (arg0 == 0) {
        far_base = (char *)&D_8007AFF0;
        if ((*(u16 *)(*(s32 *)(far_base + 0x25388) + 8) & 0x1000) == 0 ||
            *(s32 *)(far_base + 0x2549C) == 5 ||
            *(s32 *)(far_base + 0x2549C) == 6 ||
            *(s32 *)(far_base + 0x2549C) == 7 ||
            *(s32 *)(far_base + 0x2549C) == 8 ||
            *(s32 *)(far_base + 0x2549C) == 9 ||
            (var_s0 = 0x3F, *(s32 *)(far_base + 0x2549C) == 0xE)) {
            var_s0 = 0x7F;
        }
    } else {
        far_base = (char *)&D_8007AFF0;
        var_s0 = arg0;
    }
    OpenSoundSequencesAndStart();
    far_base2 = (char *)&D_8007AFF0;
    PlaySoundSequenceWithReverb(0, 0, *(s16 *)(far_base2 + 0x2549C), var_s0);
    return 1;
}
