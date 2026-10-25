#ifndef OVL30_RECORD_MENUS_H
#define OVL30_RECORD_MENUS_H

#include "common.h"
#include "game_types.h"

/* Descriptor-driven debug menus. count must be positive; the caller supplies
 * a valid gOvl30RecordMenuCursor in [0,count), aligned writable value pointers
 * for widths 1/2/4, and readable display strings. Each call prints the nine-row
 * page containing the cursor (blank rows beyond count), then handles input.
 * Directional repeatButtons move one row with wrap, or nine rows with clamp;
 * navigation plays effect 5 and suppresses editing for that call. A new pad-0
 * 0x100 press publishes the descriptor's signed id to
 * gOvl30RecordMenuSelectedId. id == -2 disables editing, not selection.
 * Neither routine initializes the shared cursor/selected-id state.
 * Both edit live data immediately; neither returns a selection value. */

/* Value menu: pad-0 repeat masks 0x20/0x40 add +/-1; 0x10/0x80 add +/-10,
 * increased to +/-100 by pad-0 held 0x100, or +/-1000 by pad-1 held 8.
 * A zero lower bound selects unsigned byte/halfword loads; any other lower
 * bound selects signed loads. Crossing upper/lower replaces the value with
 * the opposite bound, discarding overshoot; it is not saturation/modulo.
 * Width zero displays a label only; width four arithmetic remains signed.
 * Requires meaningful ordered bounds and no signed addition overflow. */
void Ovl30DrawAndEditValueMenu(Ovl30ValueMenuEntry *entries, s32 count);

/* Flag menu: repeat mask 0x20 sets the descriptor mask; 0x40 clears it.
 * Widths 1/2 truncate the stored mask; width 0 is display-only. */
void Ovl30DrawAndEditFlagMenu(Ovl30FlagMenuEntry *entries, s32 count);

/* Shared by the widgets and record-specific pages. Width 1/2 loads unsigned
 * when lower == 0 and signed otherwise; width 4 always uses signed arithmetic.
 * Add delta, replace values above upper with lower or below lower with upper,
 * then store at the selected width. Width 0/unsupported widths do not store.
 * The field must be aligned for its width; no overflow/range check is added. */
void Ovl30StepFieldValue(u8 *value, s32 width, s32 upper, s32 lower, s32 delta);

#endif
