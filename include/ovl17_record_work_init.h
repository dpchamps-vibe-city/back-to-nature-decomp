#ifndef OVL17_RECORD_WORK_INIT_H
#define OVL17_RECORD_WORK_INIT_H

#include "common.h"

/* Scene-entry initialization of the shared record work area. Seeds controls
 * and six selection indices, expands a 3x6 byte roll-threshold grid, fills six
 * row-target triples, resets card display rows and resets the timer HUD.
 * The horizontal scroll position has 12 fractional bits and starts at
 * 0x40000 (64 pixels); later scene code advances/clamps it. Does not load
 * sprite resources. */
void Ovl17InitRecordWork(void);

/* Reload all six display rows from the current shared card-table mode.
 * Modes 0..2 select that row directly; mode 3 uses row 2 and substitutes the
 * special slot's x/y in display row 2. Other modes are unchecked. Resets row
 * flags/counters/values and installs pointers to the prepared target triples.
 * Coordinates/attributes are copied without conversion. */
void Ovl17ResetCardRows(void);

#endif
