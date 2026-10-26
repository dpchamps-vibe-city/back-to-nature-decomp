#ifndef OVL30_DEBUG_FLAGS_SOUND_H
#define OVL30_DEBUG_FLAGS_SOUND_H

#include "common.h"

/* Overlay-30 page callback. Pass 0 to initialize, then feed each returned
 * state back on subsequent frames: 1 edits/displays, 999 waits for a sequence
 * load, 1000 waits for a sound-bank change. Other states pass through.
 * Reads pad 0; edits work-area flag words and the pending debug setting;
 * prints rows and can replace playing effects, sequences and CD audio.
 * The 15-row label/status tables and sound driver must already be initialized.
 * Sequence decrement wraps 0 to 35, while increment wraps 36 to 0: intentional
 * preservation of the original asymmetry, not a normalized range check. */
s32 Ovl30UpdateDebugFlagsAndSound(s32 state);

#endif
