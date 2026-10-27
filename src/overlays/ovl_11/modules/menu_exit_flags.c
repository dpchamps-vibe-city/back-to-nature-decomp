/*
 * menu_exit_flags — reconstructed translation unit.
 *
 * Clear shared work flags 26/27 and post bit 6 of the primary-object status
 * halfword. Callers use this leaf when leaving selection, calendar, options
 * and item-transfer menus; no menu-private state is owned here.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_menu_exit_flags.h"

void Ovl11PostMenuExitFlags(void) {
    char *work = (char *)&D_8006C838;

    *(s32 *)(work + 0xC) &= 0xE7FFFFFF;
    *(u16 *)(work + 0x51FE) |= 0x40;
}
