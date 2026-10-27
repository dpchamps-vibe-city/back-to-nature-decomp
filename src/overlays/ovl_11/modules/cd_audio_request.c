/* Single-slot deferred CD-audio request. The area-frame dispatcher consumes
 * this shared halfword when playback and the work-state gates permit it. */
#include "common.h"
#include "ovl11_cd_audio_request.h"

void Ovl11RequestCdAudioTrack(s16 trackIndex) {
    s16 *stateHalfwords = (s16 *)D_8006C838;
    stateHalfwords[0x4CF3] = trackIndex;
}
