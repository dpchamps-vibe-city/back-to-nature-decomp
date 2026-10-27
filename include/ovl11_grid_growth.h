#ifndef OVL11_GRID_GROWTH_H
#define OVL11_GRID_GROWTH_H

#include "common.h"

/* Set all 49 alternate-grid cells to kind/next-kind 0x167 with zero state.
 * Does not enable the alternate grid or alter the main grid. */
void Ovl11ResetAlternateAreaGrid(void);

/* One day-change pass: visit the 25x45 main grid, then the 7x7 alternate
 * grid only when work +44D4 is nonzero. Apply main-grid seasonal transitions,
 * normalize kind pairs and advance flag-0x40 items' stage/day counters from
 * the loaded group table. Season mismatch can replace kinds with 0x168.
 * State flag 0x8000 skips one day increment and is consumed; day increments
 * narrow to u8 before the stage threshold test. Main-grid kind 0x36 can also
 * refresh neighboring variants. Shared item/group/grid tables must be loaded,
 * group/stage indices valid. This may consume RNG through transition helpers;
 * it is not idempotent and is neither elapsed-time nor per-frame service. */
void Ovl11AdvanceSeasonalGridGrowth(void);

#endif
