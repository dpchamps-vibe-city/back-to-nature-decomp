#ifndef OVL11_GRID_CANDIDATE_SEARCH_H
#define OVL11_GRID_CANDIDATE_SEARCH_H

#include "common.h"

/* Search a complete grid circularly from one Rand(cellCount) start.
 * Nonzero useMainGrid selects the 45-column x 25-row main area; zero selects
 * the 7x7 alternate grid, returning 1 immediately when its enable field is 0.
 * On success return 0 and write zero-based column/row; return 1 on failure
 * without touching either output. Both pointers must be writable on success.
 *
 * All candidates require Ovl11ItemHasFlag40(item) and item != 0x36.
 * Nonzero requireMappedType additionally excludes 0x31..0x35/0x178..0x179
 * and compares the cell's +4 byte against the mapped item-type table value.
 * This takes precedence over requireKind168Or16A. Otherwise the latter
 * restricts +2 to 0x168/0x16A; its zero case permits 0x168..0x16B.
 * Item/type tables must be initialized and their indices valid; none is
 * bounds-checked. Exactly one random value is consumed per enabled search,
 * not per candidate. No grid writes or retained search cursor. ovl_11 resident. */
s32 Ovl11FindCandidateGridCell(s16 *columnOut, s16 *rowOut, s32 useMainGrid,
                             s32 requireMappedType, s32 requireKind168Or16A);

#endif
