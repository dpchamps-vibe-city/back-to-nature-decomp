#ifndef OVL30_DEBUG_MENU_LIFECYCLE_H
#define OVL30_DEBUG_MENU_LIFECYCLE_H

#include "common.h"

/* Overlay 30 debug_menu module: lifecycle entry points. Its flags/sound
 * callback and font setup retain the companion ovl30_debug_flags_sound.h and
 * ovl30_debug_font.h interfaces. Reset clears only the lifecycle state and
 * pending-change word; it does not initialize every page's state. */
void Ovl30ResetDebugMenu(void);
s32 Ovl30GetDebugMenuState(void);

/* Call once per UI update with a valid render context, graphics heap, pad
 * snapshot, text/font resources and the 18-entry callback table. State 0
 * saves the display to heap+0x4290; 2 sets up the font and fades; 3 dispatches
 * a page and handles exit; 4 restores the display and requests scene 2.
 * Page index is clamped to 0..17, not wrapped. The callback receives and
 * returns page-local state; switching pages resets it and selected record ID.
 *
 * The first page seeds/edits gOvl30DebugSettingValue. Exit waits for screen
 * fade completion, then publishes a changed value in
 * gDebugMenuRequestedSelection and sets flag 0x9C. The area-mode consumer
 * applies it and clears 0x9C; page flag/sound edits are not deferred this way.
 * Flag 0x9B reflects the separate pending-change word. Fade, master volume,
 * font state, packet/work flags and CD-audio polling are affected. Shared
 * display scratch makes this service non-reentrant; do not reuse it until
 * the exit restoration. No bounds checks on heap capacity or callback state. */
void Ovl30UpdateDebugMenu(void);

#endif
