#ifndef OVL11_SCENE_ANIMATION_CYCLE_H
#define OVL11_SCENE_ANIMATION_CYCLE_H

/* ovl_11 must be resident. A three-record vertex-blend/visibility cycle,
 * using shared pointers and phase state. These calls do not draw. */

/* Scan fifteen scene records for flag 0x100; their low flag nibble selects
 * slots 0,1,2, with blend durations 8,4,12 updates. Last match wins. Reset
 * progress and phase; show slot 0 and hide slots 1/2. Other nibbles retain
 * the original slot-0/zero-duration fallback without changing visibility.
 * Scene data must supply all three records and valid blend pointers before
 * advancing; only slot 0 is checked by Step. Re-init after pool reuse.
 * Original identity: ovl_11:0x800DD374. */
void Ovl11InitSceneAnimationCycle(void);

/* Idle waits for game flag 7, then completes blends in order 0 -> 1 -> 2.
 * Each active call increments progress BEFORE testing the signed duration.
 * At an endpoint, reset outgoing/incoming progress, hide the outgoing record
 * and show the incoming one. Completion returns to slot 0/idle and clears
 * flag 7. Release during the cycle does not cancel it. A trigger call changes
 * phase but does not advance progress. Non-reentrant; no elapsed-time input.
 * Original identity: ovl_11:0x800DD4D8. The transition helper at 800DD45C is
 * nested in this function, not an independently callable public interface. */
void Ovl11StepSceneAnimationCycle(void);

#endif
