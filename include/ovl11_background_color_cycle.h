#ifndef OVL11_BACKGROUND_COLOR_CYCLE_H
#define OVL11_BACKGROUND_COLOR_CYCLE_H

/* ovl_11 must be resident. These calls write the background RGB and enable
 * background clearing in BOTH render contexts. They do not submit GPU work.
 * Scene kind 44 uses this cycle through the scene setup/update dispatchers.
 * The private index/direction make it non-reentrant; no elapsed-time input. */

/* Reset to index 511, increasing direction, and RGB (0,165,213).
 * Original identity: ovl_11:0x800DDB64. */
void Ovl11ResetBackgroundColorCycle(void);

/* Write RGB = (low-high)*(511-index)/511 + high componentwise, then move
 * the index by one and reverse at the clamped endpoints 0 and 511.
 * low=(0,99,127), high=(0,165,213); integer division truncates toward zero.
 * Each endpoint is written on two consecutive calls at a reversal.
 * Original identity: ovl_11:0x800DDBA0. */
void Ovl11StepBackgroundColorCycle(void);

#endif
