#ifndef OVL30_DEBUG_MENU_LIFECYCLE_H
#define OVL30_DEBUG_MENU_LIFECYCLE_H

#include "common.h"

/* Overlay 30 lifecycle entry points. Reset clears only the lifecycle state
 * and pending-change word; it does not initialize every page's state. */
void Ovl30ResetDebugMenu(void);
s32 Ovl30GetDebugMenuState(void);

/* Call once per UI update with a valid render context, graphics heap, pad
 * snapshot, text/font resources and the 18-entry callback table. State 0
 * saves the display to heap+0x4290; 2 sets up the font and fades; 3 dispatches
 * a page and handles exit; 4 restores the display and requests scene 2.
 * Page index is clamped to 0..17, not wrapped. The callback receives and
 * returns page-local state; switching pages resets it and selected record ID.
 *
 * Exit waits for screen fade completion, then copies the edited setting
 * when changed and sets game flags 0x9C/0x9B as observed. Fade, master volume,
 * font state, packet/work flags and CD-audio polling are affected. Shared
 * display scratch makes this service non-reentrant; do not reuse it until
 * the exit restoration. No bounds checks on heap capacity or callback state. */
void Ovl30UpdateDebugMenu(void);

#endif
