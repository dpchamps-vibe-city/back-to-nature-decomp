#ifndef MASTER_VOLUME_H
#define MASTER_VOLUME_H

#include "common.h"

/* Master-volume ramp state uses signed 16.16 levels (0..127) and one step
 * per service call, not elapsed time. Left/right master levels are identical. */

/* Set the level and target to 127, idle the ramp, clear the step and mark
 * sound transfer complete. Does NOT send a volume command to the SDK. */
void InitMasterVolumeRamp(void);

/* Poll the pending sound-transfer driver, latch completion when it is idle,
 * then step the volume ramp if active. Called by the regular frame service. */
void UpdateMasterVolumeAndSoundTransfer(void);

/* Nonzero completion latch; a sound request clears it outside this module.
 * Independent of whether a volume ramp is active. */
s32 IsSoundTransferComplete(void);

/* Begin a ramp without resetting the current level. ticks must be nonzero.
 * Fade-in step = full scale / ticks, NOT (full scale - current) / ticks;
 * fade-out step = -current / ticks. Integer division can produce a zero step
 * that never completes. Positive ticks are the intended use. */
void BeginMasterVolumeFadeIn(s32 ticks);
void BeginMasterVolumeFadeOut(s32 ticks);

/* 0=idle, 1=fade in, 2=fade out. Fade-out stops at level <=0; fade-in stops
 * only after exceeding 127, so landing exactly at 127 stays active until
 * another step. There is no exact-duration guarantee after truncation. */
s32 GetMasterVolumeRampState(void);

#endif
