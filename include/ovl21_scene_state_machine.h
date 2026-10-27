#ifndef OVL21_SCENE_STATE_MACHINE_H
#define OVL21_SCENE_STATE_MACHINE_H

#include "common.h"

/* Engine-installed per-frame callback for overlay 21; no arguments or result.
 * gSceneSubphase indexes five phases: resource/sound loading, fade-in wait,
 * nineteen-state interaction update, exit fade request, and exit completion.
 * It temporarily selects text draw mode 3 and restores the previous mode.
 * Phase and state indices are unchecked and must be initialized by scene entry.
 *
 * The interaction advances message windows, polls pad input, updates six
 * records (two groups of three), invokes simulation/drawing, and maintains
 * sound-effect handles. Voice completion waits are capped at 150 updates,
 * while message-window completion is a separate gate. Counters use update
 * calls, not elapsed milliseconds; the main interaction timer is seeded with
 * its configured value times 30. This does not establish real-time units.
 *
 * Exit waits for both screen and volume fades, stops saved effect handles and
 * sequence 0, rewinds sound-load slots, marks phase FF, requests resident
 * scene 6 and sets shared-work flag 0x80000. Shared scene control, message,
 * rendering and sound services are mutable and non-reentrant.
 */
void Ovl21StepScene(void);

#endif
