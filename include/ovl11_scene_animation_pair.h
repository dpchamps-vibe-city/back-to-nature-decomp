#ifndef OVL11_SCENE_ANIMATION_PAIR_H
#define OVL11_SCENE_ANIMATION_PAIR_H

/* ovl_11 must be resident. This controller changes vertex-blend progress
 * and the renderer's 0x1000 hide bit on two records; it does not draw.
 * The renderer interpolates vertices using progress/duration. */

/* Select records with flag 0x80 from the fifteen-record scene pool. Their
 * low flag nibble selects slot 0 or 1, with durations 8 and 12 update steps.
 * Last match wins. Other nibbles select slot 0 with zero duration: scene data
 * must supply the intended 0/1 records and valid blend pointers. Subsequent
 * transitions assume both slots exist; only slot 0 is checked by Step.
 * Initially reset progress, show slot 0, hide slot 1, and clear game flag 9.
 * If flag 10 is already set, instead hold slot 1 at its final blend value.
 * Original identity: ovl_11:0x800DD5B0. */
void Ovl11InitSceneAnimationPair(void);

/* Idle waits for flag 9, then advances slot 0 by one per call until its
 * duration. At that endpoint, flag 9 switches visibility to slot 1, resets
 * both progress values, and is cleared. Independently, flag 10 starts slot
 * 1's blend; this leg clamps at its duration, then holds until flag 10
 * clears. Clearing it restores slot 0, resets both values and returns idle.
 * Without flag 10 the controller remains in the slot-0 phase, even after
 * switching visibility. Resetting slot 0 postpones the next flag-10 test
 * until its full duration has elapsed again, now while slot 0 is hidden.
 * State/pointers are shared and non-reentrant; init again after pool reuse.
 * Original identity: ovl_11:0x800DD6F0. */
void Ovl11StepSceneAnimationPair(void);

#endif
