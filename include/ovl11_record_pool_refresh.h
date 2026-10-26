#ifndef OVL11_RECORD_POOL_REFRESH_H
#define OVL11_RECORD_POOL_REFRESH_H

#include "common.h"
#include "game_types.h"

/* Original ovl_11:800F5404. Refresh bit 0 of selected records in work lists
 * +DD8C/+DD90 from status masks and lists +DD94/+DD98 from byte arrays.
 * Other flag bits are preserved. Calls the external fixed-list refresh,
 * then, when far-work +25476 == 2, applies coordinate rules to +DD90.
 * Rule selector work+44CE: 1/2 use their ten rules; all other values use
 * two defaults. The first matching kind replaces x/y/z verbatim; a miss
 * leaves coordinates unchanged. Lists/counts and byte arrays must be valid;
 * the byte-array views contain 22 entries and no bounds check is performed. */
void Ovl11RefreshDispatchRecordPools(void);

/* Original ovl_11:800F5700. Borrow the first record whose u16 kind equals
 * the signed kind argument, or return null. Negative kinds cannot match.
 * A nonpositive count reads no records. No flags or other state are changed. */
Ovl11DispatchRecord *Ovl11FindDispatchRecordByKind(s16 kind,
                                               Ovl11DispatchRecord entries[],
                                               s32 count);

#endif
