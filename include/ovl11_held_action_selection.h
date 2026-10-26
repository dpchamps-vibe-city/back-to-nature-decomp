#ifndef OVL11_HELD_ACTION_SELECTION_H
#define OVL11_HELD_ACTION_SELECTION_H

#include "common.h"
#include "game_types.h"

/* Advance the object's held-input selection action once; always returns 0.
 * Entry phase 0x45 selects an area category and consumes the shared signal.
 * Holding input bit 0x80 runs delay/pulse countdowns, sprite animation and pad
 * feedback. Release follows the object's animation-completion path; one path
 * chooses a weighted outcome and passes a three-halfword item descriptor to
 * the object-state service. Object flag 0x40 pauses progress; animation flag
 * 0x100 marks completion on the release paths.
 *
 * Uses shared work/selection/projection state and the active render context:
 * not reentrant, and not a standalone object initializer. The sprite source,
 * action position/header and signal must already be installed by the caller's
 * action setup. Category and table indices are unchecked. Countdown units are
 * update calls. Outcomes can set one-shot game flags and shared work flags;
 * rejected weighted rows retain the same RNG roll rather than rerolling. */
s32 Ovl11UpdateHeldSelectionAction(Ovl11SelectionObject *object);

#endif
