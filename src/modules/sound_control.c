/*
 * Game-facing sound control, exe:8001FABC–8001FE7C.
 *
 * Effect/sequence adapters, queued bank/sequence loads and master-volume
 * ramps share the completion lifecycle in gMasterVolumeState. Every load
 * command clears its latch; the frame service advances the low-level queue
 * and latches completion only after observing it empty. Ramp completion is
 * independent, although the same service advances both operations.
 *
 * sound_commands.h publishes commands; master_volume.h publishes frame
 * service, completion and signed 16.16 ramps. Both require initialized driver
 * state and caller-provided read-buffer capacity. The two GP-owned command
 * globals belong here; the low-level driver's slot tables remain external.
 * Membership and outer-boundary evidence: notes/file-groupings.md.
 */

#include "common.h"
#include "sound_commands.h"
#include "master_volume.h"
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

/* Shared control record: transfer completion and master-volume integration.
 * Initialization marks the transfer idle but does not issue an SDK volume
 * command. Its 24-byte extent exceeds -G8, so absolute addressing gives no
 * TU-ownership evidence. The two small command words retain their witnessed
 * GP definitions above. */
void StepMasterVolumeRamp(void);

void InitMasterVolumeRamp(void) {
    MasterVolumeState *state;
    s32 fullScale;
    s32 complete;

    state = &gMasterVolumeState;
    fullScale = 0x7F0000;
    complete = 1;
    *(s32 *)state->reserved = 0;
    state->rampState = 0;
    state->level16 = fullScale;
    state->step16 = 0;
    state->fullScale16 = fullScale;
    state->transferComplete = complete;
}

void UpdateMasterVolumeAndSoundTransfer(void) {
    if (GetPendingSoundLoad() != -1) {
        StepSoundLoad();
    } else {
        gMasterVolumeState.transferComplete = 1;
    }
    if (gMasterVolumeState.rampState != 0) {
        StepMasterVolumeRamp();
    }
}

s32 IsSoundTransferComplete(void) {
    return gMasterVolumeState.transferComplete != 0;
}

void StepMasterVolumeRamp(void) {
    s32 volume;
    s32 nextLevel;
    s32 roundedLevel;
    s32 fullScale;

    nextLevel = gMasterVolumeState.level16 + gMasterVolumeState.step16;
    gMasterVolumeState.level16 = nextLevel;
    if (nextLevel <= 0) {
        gMasterVolumeState.level16 = 0;
        gMasterVolumeState.rampState = 0;
    }
    fullScale = 0x7F0000;
    if (gMasterVolumeState.level16 > fullScale) {
        gMasterVolumeState.level16 = fullScale;
        gMasterVolumeState.rampState = 0;
    }
    roundedLevel = gMasterVolumeState.level16;
    if (roundedLevel < 0) {
        roundedLevel += 0xFFFF;
    }
    volume = roundedLevel >> 16;
    SsSetMVol(volume, volume);
}

void BeginMasterVolumeFadeIn(s32 ticks) {
    MasterVolumeState *state;
    s32 step;
    s32 active;

    state = &gMasterVolumeState;
    step = state->fullScale16;
    step = step / ticks;
    state->step16 = step;
    active = 1;
    state->rampState = active;
}

void BeginMasterVolumeFadeOut(s32 ticks) {
    gMasterVolumeState.step16 = -gMasterVolumeState.level16 / ticks;
    gMasterVolumeState.rampState = 2;
}

s32 GetMasterVolumeRampState(void) {
    return gMasterVolumeState.rampState;
}
