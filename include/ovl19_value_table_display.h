#ifndef OVL19_VALUE_TABLE_DISPLAY_H
#define OVL19_VALUE_TABLE_DISPLAY_H

#include "common.h"

/* Draw the overlay-19 display state's two columns of four text labels.
 * Positions are screen pixels: X=21/221, Y=57/105/150/195. The first cell
 * uses the shared word at exe:8007AE58 as a text address; other cells select
 * from gOvl19ValueLabelText via clamped signed value indices (0..19).
 *
 * Saves the engine text setting, uses setting 1 for labels, and restores it.
 * The signed count at display-state +8 enables additional sprite markers
 * (>0 for the first set, >=2 for the second). Uses the active render context
 * and packet-work allocator, and changes gOvl19UiSprite animation/frame state.
 * The nested sprite helper is an implementation detail, not a public API. */
void Ovl19DrawValueTable(void);

#endif
