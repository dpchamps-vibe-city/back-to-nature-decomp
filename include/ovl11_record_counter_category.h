#ifndef OVL11_RECORD_COUNTER_CATEGORY_H
#define OVL11_RECORD_COUNTER_CATEGORY_H

#include "common.h"

/* ovl_11:800F02C8. Read the unsigned +4 counter of the five event records
 * named by gOvl11CounterCategoryRecordIds. Return the first greatest value's
 * LIST POSITION (0..4), not its record ID, if that value is >=500; else -1.
 * No writes. Requires initialized records and valid signed IDs in the table.
 * Counter units are not established. */
s32 Ovl11SelectHighestRecordCounter(void);

#endif
