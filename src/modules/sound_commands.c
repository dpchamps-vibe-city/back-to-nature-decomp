/*
 * Game-facing sound commands, exe:8001FABC..8001FCE4.
 *
 * Voice playback/cleanup, sequence reverb presets and scene load commands
 * adapt sound_driver.h without owning its slot tables. The two GP-owned
 * command globals belong here; master-volume ramps remain separate.
 * sound_commands.h documents IDs, side effects and unchecked inputs.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "sound_commands.h"
#include "psyq/libsnd.h"
#include "sound_driver.h"

s32 PlaySoundEffect(s16 soundId) {
    return PlaySoundEffectInVoiceRange(soundId, 0, 0x17);
}

void StopSoundEffectVoice(s32 voice) {
    KeyOffSoundVoice(voice);
}

s32 GetLastSpuVoiceIndex(void) {
    return 0x17;
}

s32 MuteLastSpuVoice(void) {
    MuteAndKeyOffSoundVoice(0x17);
    return 0;
}

s32 PlaySoundSequenceWithReverb(s16 slot, s16 playParameter, s16 reverbPreset, s32 volume) {
    s16 depth;

    PlaySoundSequenceSlot((s32) slot, 0, (s32) (s16) volume, (s32) (s16) volume, (s16) (s32) playParameter);
    SsUtSetReverbType(gSoundCommandReverbPresets[reverbPreset * 2]);
    SsUtReverbOn();
    depth = gSoundCommandReverbPresets[(reverbPreset * 2) + 1];
    SsUtSetReverbDepth(depth, depth);
    return 0;
}

s32 StopSoundSequence(s16 slot) {
    StopSoundSequenceSlot(slot, 0);
    return 0;
}

s32 gSoundCommandReadBuffer;

void SetSoundCommandReadBuffer(s32 unused, s32 readBuffer) {
    gSoundCommandReadBuffer = readBuffer;
}

s16 gSoundCommandResetGuard;

void QueueSoundLoadCommand(s16 command, s16 unused) {
    s32 pairAddress;

    gMasterVolumeState.transferComplete = 0;
    if (command != 0x3E7) {
        if (command < 0x3E8) {
            pairAddress = command * 2;
            if (command == 0) {
                goto reset;
            }
            goto pair;
        } else {
            pairAddress = command * 2;
            if (command == 0x3E8) {
                goto bankOne;
            }
            goto pair;
        }
    } else {
        goto bankTwo;
    }
    return;
reset:
    if (gSoundCommandResetGuard == 0) {
        SsUtAllKeyOff(0);
        ClearSoundVoiceKeyOnFrames();
        ConfigureSoundLoadBuffers(&gSoundCommandLoadArea, gSoundCommandReadBuffer, 0, 0);
        QueueSoundBankLoad(0);
        gSoundCommandResetGuard = 0;
    }
    return;
bankTwo:
    QueueSoundBankLoad(2);
    return;
bankOne:
    QueueSoundBankLoad(1);
    return;
pair:
    pairAddress = pairAddress + (s32) &gSoundCommandLoadPairs;
    QueueSoundBankLoad(((u8 *)pairAddress)[0]);
    QueueSoundSequenceLoad(((u8 *)pairAddress)[1], 2);
}

/* Unused empty entry; no behavior supports a more specific name. */
void func_8001FCDC(void) {
}
