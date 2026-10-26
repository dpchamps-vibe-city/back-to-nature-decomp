/*
 * menu_panel — reconstructed translation unit.
 *
 * Legacy six-argument menu panel submission facade; forwards pixel geometry
 * to DrawBeveledPanel and ignores the mode argument.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "beveled_panel.h"

void DrawMenuPanel(u32 *ot, s32 ignoredMode, s32 x, s32 y, s16 width, s16 height) {
    DrawBeveledPanel(ot, (s16)x, (s16)y, width, height);
}
