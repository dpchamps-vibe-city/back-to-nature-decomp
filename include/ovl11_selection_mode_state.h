#ifndef OVL11_SELECTION_MODE_STATE_H
#define OVL11_SELECTION_MODE_STATE_H

#include "common.h"

/*
 * Public entries to selection_menu_state (ovl_11:800FFDCC..80103770).
 * The module owns the record-grid, flag-slot and item-grid handler chain;
 * ovl11_selection_candidate.h publishes its query/trial/result interface.
 * Only one selection transaction may be active: cursor, phase and work-set
 * storage is shared, not supplied by the caller.
 * Both entries mark resident control bit 0x20000, reset the mode work state
 * and texture-cell cache, disable portraits/text input, then request archive
 * 3 message 0x3B5. Default/Alternate differ only in the origin byte (0/1)
 * and initial handler (entry prompt/record grid); no gameplay interpretation
 * implied. Exiting clears the phase, sets resident control bit 0x40000 and
 * work request bit 0x2000, runs the area refresh hook and resets messages.
 * Requires initialized message, texture-stream, render and resident work state.
 */
void Ovl11EnterSelectionModeDefault(void);
void Ovl11EnterSelectionModeAlternate(void);

/* Per-frame service: draw the sprite grid, wait for the entry message/texture
 * stream or CD-audio stop as required, then invoke the installed handler.
 * On handler changes reset four cursor bytes. Calls run with text palette 3
 * and pen offsets (1,2); palette is restored, pen offsets become (0,2).
 * An active dispatch state requires a non-null installed handler. */
void Ovl11ServiceSelectionMode(void);

#endif
