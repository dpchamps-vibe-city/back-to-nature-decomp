/*
 * Object-program sound/transition commands. The sound-bank controller calls
 * all three adjacent command entries; its scene client also composes their
 * fade/start handshake. Original order: ovl_11:E6B18..E6DFC exclusive.
 * See ovl11_scene_sound_commands.h and notes/file-groupings.md.
 */
#include "common.h"
#include "sound_driver.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "screen_fade.h"
#include "ovl11_scene_sound_commands.h"

s32 ovl_11_func_800E63C8(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

/* The original command dispatch and EC52C both pass four operands; reserved
 * is unread. Publishing that complete contract also covers direct callers. */
s32 Ovl11RequestSceneSoundBank(s16 soundBank, s32 beginFade, s32 sequenceMode, s32 reserved) {
    char *farWork;

    if ((beginFade << 0x10) != 0) {
        Ovl11ScreenFadeCommand(1, 0x30, 1, 0);
    }
    farWork = (char *)&D_8007AFF0;
    if ((*(s32 *)(farWork + 0x2549C)) != soundBank) {
        gObjectSoundBankRequest = soundBank;
        goto pending;
    }
    if (sequenceMode == 1) {
        Ovl11StartSceneSoundSequence(0, 0, 0, 0);
        return 1;
    }
    if (sequenceMode == -1) {
        Ovl11StopSceneSoundSequence(0, 2, 0, 0);
        return 1;
    }
    return 1;
pending:
    return 0;
}

s32 Ovl11StartSceneSoundSequence(s16 volume, s32 reserved1, s32 reserved2, s32 reserved3) {
    char *settingsWork;
    char *playbackWork;
    s16 selectedVolume;

    if (volume == 0) {
        settingsWork = (char *)&D_8007AFF0;
        if ((*(u16 *)(*(s32 *)(settingsWork + 0x25388) + 8) & 0x1000) == 0 ||
            *(s32 *)(settingsWork + 0x2549C) == 5 ||
            *(s32 *)(settingsWork + 0x2549C) == 6 ||
            *(s32 *)(settingsWork + 0x2549C) == 7 ||
            *(s32 *)(settingsWork + 0x2549C) == 8 ||
            *(s32 *)(settingsWork + 0x2549C) == 9 ||
            (selectedVolume = 0x3F, *(s32 *)(settingsWork + 0x2549C) == 0xE)) {
            selectedVolume = 0x7F;
        }
    } else {
        settingsWork = (char *)&D_8007AFF0;
        selectedVolume = volume;
    }
    OpenSoundSequencesAndStart();
    playbackWork = (char *)&D_8007AFF0;
    PlaySoundSequenceWithReverb(0, 0, *(s16 *)(playbackWork + 0x2549C), selectedVolume);
    return 1;
}

s32 Ovl11StopSceneSoundSequence(s16 ticks, s16 mode, s32 reserved2, s32 reserved3) {
    s32 complete;

    complete = 0;
    if (mode == 0) {
        BeginMasterVolumeFadeOut(ticks ? ticks : 10);
    } else if (GetMasterVolumeRampState() == 0 || mode == 2) {
        StopSoundSequence(0);
        complete = 1;
    }
    return complete;
}

s32 Ovl11ScreenFadeCommand(s16 mode, s16 ticks, s32 begin, s32 additiveBlend) {
    s32 sceneOt;
    s32 blendMode;
    void (*startFade)(s32, s32, s32);

    if (begin != 0) {
        sceneOt = 0;
        if (mode != 2) {
            blendMode = 2;
            if (mode >= 3) {
                if (mode != 3) {
                    /* Unknown modes keep the frame ordering table. */
                } else {
                    goto sceneTarget;
                }
            }
        } else {
            ovl_11_func_800E63C8(0, 0, 0, 0);
sceneTarget:
            sceneOt = 1;
            blendMode = 2;
        }
        if (additiveBlend != 0) {
            blendMode = 1;
        }
        if (mode & 1) {
            startFade = BeginScreenFadeDecrease;
        } else {
            startFade = BeginScreenFadeIncrease;
        }
        startFade((s32)ticks, sceneOt, blendMode);
        return 0;
    }
    return IsScreenFadeComplete() == 1;
}
