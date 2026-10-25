#ifndef AUX_SPRITE_DISPLAY_H
#define AUX_SPRITE_DISPLAY_H

#include "common.h"

/*
 * Resident auxiliary sprite display with its own sprite source, two 32-entry
 * ordering tables and packet workspace. It SHARES the main render contexts
 * and active-context index; it is not an independent framebuffer renderer.
 */

/* exe:0x8001AFE0. Clear the 0x1300-byte OT/packet region, hide display output,
 * load the embedded sprite image/source and enable auxiliary presentation.
 * Used when entering the relevant main-loop scene branches. */
void ResetAuxSpriteDisplay(void);

/* exe:0x8001B100 / 0x8001B10C. Disable immediately, or read the raw enable
 * flag without changing state. Disable does not erase queued packets or
 * restore the main render context. Initialization is what enables rendering. */
void DisableAuxSpriteDisplay(void);
s32 IsAuxSpriteDisplayEnabled(void);

/* exe:0x8001B118. Called by the resident service hook alongside CD polling.
 * If enabled and not already rendering: synchronize, unmask display, submit
 * the current auxiliary OT, switch the shared draw/display context, clear
 * that context's auxiliary OT and build the next sprite at (160,120) pixels.
 * A recursive call is ignored. A disabled call has no effects. */
void PresentAuxSpriteDisplay(void);

#endif
