#ifndef OVL11_AREA_ENTRY_CACHE_H
#define OVL11_AREA_ENTRY_CACHE_H

#include "common.h"

/* Clear the saved area-entry reference at secondary work +25390 and set
 * its two reload slots (+25484/+25488) to the positive word value 65535,
 * NOT signed -1. Called during area startup; the light/full entry paths
 * repeat these same resets along with their additional initialization.
 * Does not reset the mode/entry-step machines, load assets or allocate work. */
void Ovl11ResetAreaEntryCache(void);

#endif
