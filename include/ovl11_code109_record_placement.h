#ifndef OVL11_CODE109_RECORD_PLACEMENT_H
#define OVL11_CODE109_RECORD_PLACEMENT_H

#include "common.h"

/* Independently attempt three small-record allocations with code 0109,
 * requested index -1 and mode 0. Successful records receive +30 = 48,
 * +22 = the table index (0..2), and three position words at +38/+3C/+40.
 * Positions use the record system's coordinate units. An allocation failure
 * skips that position without aborting later attempts. No rollback, retry or
 * duplicate check is performed; repeated calls may allocate duplicates. */
void Ovl11PlaceCode109Records(void);

#endif
