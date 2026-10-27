#ifndef OVL11_SELECTED_RECORD_FLAG_GUARD_H
#define OVL11_SELECTED_RECORD_FLAG_GUARD_H
#include "common.h"

/* For a record with u16 ID at +0 and s32 flags at +34, clear active bit 20000
 * only if set and its ID differs from the SIGNED selected-item ID. Other bits
 * and records matching that ID are unchanged. No null/extent checks. */
void Ovl11DeactivateUnselectedRecord(s32 *record);
#endif
