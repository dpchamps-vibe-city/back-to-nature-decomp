#ifndef OVL11_SCORED_SCENE_SEQUENCE_H
#define OVL11_SCORED_SCENE_SEQUENCE_H

#include "common.h"

/* Object-command phase machine. All four operands name unchecked value slots;
 * phaseSlot == -1 instead places five entities and returns 1 without slot I/O.
 * Otherwise the phase is read as a signed low halfword and written as a word.
 *
 * Phase 0 admits the current item, writes itemIdSlot/itemValueSlot, starts the
 * item operation and returns admission success WITHOUT advancing the phase.
 * Phase 1 stages records and selects among five randomized signed-halfword
 * scores. Shared-work+44B8 selects the row (upper-clamped to 4, not lower-
 * clamped); it must be nonnegative. A submitted item's value can select 0x29.
 * Score candidate index 1 leaves selectedRecordSlot unchanged; initialize it.
 *
 * Phases 2–27 coordinate messages, raw-coordinate movement, camera selection,
 * fades and audio. Service those asynchronous systems separately. A completed
 * step advances the phase, skipping 14–16 after 13 when itemIdSlot holds zero.
 * Return is suppressed until the NEW phase is 20 or >=28 (in addition to the
 * two early-return paths above). This is not a uniformly boolean whole-scene
 * completion API. Slot aliases retain ordered overwrite behavior.
 */
s32 Ovl11StepScoredSceneSequence(s16 phaseSlot, s16 itemIdSlot,
                                s32 itemValueSlot, s32 selectedRecordSlot);

#endif
