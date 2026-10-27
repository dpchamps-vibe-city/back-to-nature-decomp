/*
 * season_mask — reconstructed translation unit.
 *
 * Calendar-season bit adapter for placement filters. Returns one of the four
 * low bits; invalid seasons select the first bit. This read-only service
 * shares no private state or helper with its neighbors.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_season_mask.h"

s32 Ovl11GetCalendarSeasonMask(void) {
    switch (gCalendarSeason) {
    case 0:
    default:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 8;
    }
}
