/*
 * cd_audio_command — reconstructed translation unit.
 *
 * Message-window-gated CD/XA command: request a track or poll CD audio idle
 * state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "cd_audio.h"
#include "ovl11_cd_audio_command.h"

s32 Ovl11CdAudioCommand(s32 trackIndex, s32 reserved1, s32 begin,
                       s32 reserved3) {
    s32 complete;

    trackIndex = (s16)trackIndex;
    if (gMessageWindowPhase != 0) {
        return 1;
    }
    complete = 0;
    if (begin != 0) {
        StartCdAudioTrack(trackIndex);
    } else if (GetCdAudioState() == 0) {
        complete = 1;
    }
    return complete;
}
