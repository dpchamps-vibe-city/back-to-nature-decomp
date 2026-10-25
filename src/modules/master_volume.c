/*
 * Master-volume ramps and sound-transfer completion, exe:8001FCE4..8001FE7C.
 *
 * All seven members initialize, service or query the six-word volume record.
 * The record is absolute-addressed; it is not part of the sequencer's GP
 * state. master_volume.h documents its fixed-point units, truncation and
 * asymmetric endpoints. Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "master_volume.h"
#include "psyq/libsnd.h"
#include "sound_driver.h"

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
