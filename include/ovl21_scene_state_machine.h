#ifndef OVL21_SCENE_STATE_MACHINE_H
#define OVL21_SCENE_STATE_MACHINE_H

#include "common.h"

/* Public entry of modules/scene_runtime.c. Engine-installed per-frame callback
 * for resident overlay 21; no arguments or result. Do not call while another
 * overlay occupies its RAM. Render context, graphics heap, CD, GTE and sound
 * services must be initialized. Loading busy-polls without timeout and uses
 * heap scratch at +0x4290; no storage/capacity validation is supplied.
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
 * rendering and sound services are mutable and non-reentrant. Sprite/geometry
 * loading, entity views, pause and drawing are internal parts of this same
 * runtime, not independently callable public services. Retained sprite headers
 * and work storage must outlive all six-record/projectile draws.
 */
void Ovl21StepScene(void);

#endif
