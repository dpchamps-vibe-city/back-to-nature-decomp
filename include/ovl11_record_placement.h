#ifndef OVL11_RECORD_PLACEMENT_H
#define OVL11_RECORD_PLACEMENT_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Refresh records flagged 0x20000 using an existing collision tag and four
 * VECTOR words in the pools' native position units. Clears that flag, copies
 * tag/position and calls the position-query service (its result is ignored).
 * IDs 0x109/10A use the first flagged B4 entry regardless of its ID;
 * 0x161/162 use the first matching B8 entry; 0x106/107 use the singleton.
 * All other IDs update every match in the six-entry B0 pool, clearing entries
 * whose IDs are not enabled by the item-rule mask. Returns 1 even if no
 * record was found: it is not a found/success predicate. Pointers must be
 * valid when a matching record exists. Does not allocate a record.
 */
s32 Ovl11RefreshRecordById(s16 itemId, Ovl11CollisionTag *tag, VECTOR *position);

#endif
