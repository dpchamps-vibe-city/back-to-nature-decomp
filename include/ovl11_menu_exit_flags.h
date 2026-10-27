#ifndef OVL11_MENU_EXIT_FLAGS_H
#define OVL11_MENU_EXIT_FLAGS_H

#include "common.h"

/* Menu-exit notification used by the selection/calendar/options controllers.
 * Clear bits 26 and 27 of shared work +0x0C, then set bit 6 of the unsigned
 * status halfword at +0x51FE. Other bits are retained; no menu buffers, sprite
 * state or scene selection are changed. Repeated calls are idempotent.
 */
void Ovl11PostMenuExitFlags(void);

#endif
