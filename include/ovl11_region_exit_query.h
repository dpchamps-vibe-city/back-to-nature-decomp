#ifndef OVL11_REGION_EXIT_QUERY_H
#define OVL11_REGION_EXIT_QUERY_H

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/*
 * Searches two exit rectangles in the selected variant (0..3; any other
 * unsigned value selects 0). Position is passed by value in raw world units;
 * only X/Z participate, and rectangle edges are excluded. The signed region
 * is compared with the table's unsigned halfword source tag.
 * On the first match, writes the destination position (Y=0) and region, then
 * returns 1. Returns 0 without changing either output if no rectangle matches.
 * Output pointers must be valid on success. Does not mutate global state.
 */
s32 Ovl11FindRegionExit(VECTOR position, s16 sourceRegion,
                       VECTOR *destinationPosition, u16 *destinationRegion,
                       u32 variant);

#endif
