/* Shared recorded-input stream start/step service. Cursor initialization
 * belongs to the producer; the start branch deliberately has no bound test. */
#include "common.h"
#include "ovl11_recorded_input_playback.h"

#define PLAYBACK ((struct Ovl11InputPlaybackState *)D_8006C838)
#define MODE ((struct Ovl11SelectionResetMode *)&D_8007AFF0)

s32 Ovl11StepRecordedInputPlayback(void) {
    u32 sampleIndex;

    if (PLAYBACK->flags & 0x10000) {
        sampleIndex = PLAYBACK->input->index;
        if (sampleIndex < 0x738U) {
            PLAYBACK->input->current = PLAYBACK->input->samples[sampleIndex];
            ++PLAYBACK->input->index;
        } else if (MODE->mode == 0) {
            MODE->mode = 0x1A;
        }
    } else if (PLAYBACK->flags & 0x8000) {
        PLAYBACK->flags = (PLAYBACK->flags & ~0x8000) | 0x10000;
        gOvl11PrimaryHeldButtons = 0;
        gOvl11PrimaryPressedButtons = 0;
        PLAYBACK->input->current = PLAYBACK->input->samples[PLAYBACK->input->index];
        ++PLAYBACK->input->index;
        return 1;
    }
    return 0;
}
