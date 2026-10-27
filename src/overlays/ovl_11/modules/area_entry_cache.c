/*
 * area_entry_cache — reconstructed translation unit.
 *
 * Reset the area-entry far-work reference and two reload sentinels without
 * rebuilding scene services.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_area_entry_cache.h"

void Ovl11ResetAreaEntryCache(void) {
    Ovl11FarInitView *farState;

    farState = (Ovl11FarInitView *)&D_8007AFF0;
    farState->field_25390 = 0;
    /* These are word stores of 65535, not stores of an all-ones word. */
    farState->field_25484 = 0xFFFF;
    farState->field_25488 = 0xFFFF;
}
