#ifndef OVL11_CD_AUDIO_REQUEST_H
#define OVL11_CD_AUDIO_REQUEST_H

#include "common.h"

/* Replace the signed pending-track halfword at shared work+0x99E6.
 * Zero cancels the request; other indices must be valid for StartCdAudioTrack.
 * No playback starts here: Ovl11RunAreaFrameUpdates waits for idle CD audio
 * and its work-state gates, starts the selected track, then clears the slot.
 * A later request overwrites an earlier one; there is no queue or validation.
 */
void Ovl11RequestCdAudioTrack(s16 trackIndex);

#endif
