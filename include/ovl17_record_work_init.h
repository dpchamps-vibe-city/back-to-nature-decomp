#ifndef OVL17_RECORD_WORK_INIT_H
#define OVL17_RECORD_WORK_INIT_H

#include "common.h"

/* Scene-entry initialization of the shared record work area. Seeds controls
 * and six selection indices, expands a 3x6 byte roll-threshold grid, fills six
 * row-target triples, resets horse-race entrants and resets the timer HUD.
 * The horizontal scroll position has 12 fractional bits and starts at
 * 0x40000 (64 pixels); later scene code advances/clamps it. Does not load
 * sprite resources. */
void Ovl17InitRecordWork(void);

/* Reload six entrants from the shared horse-race table.
 * Modes 0..2 select that race directly; mode 3 uses race 2 and substitutes
 * the player's horse parameters in entrant 2. Other modes are unchecked.
 * Resets flags/counters/values and installs prepared target-triple pointers.
 * Race parameters/attributes are copied without conversion. */
void Ovl17ResetHorseRaceEntrants(void);

#endif
