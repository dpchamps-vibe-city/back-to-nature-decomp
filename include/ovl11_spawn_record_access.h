#ifndef OVL11_SPAWN_RECORD_ACCESS_H
#define OVL11_SPAWN_RECORD_ACCESS_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Refresh pool state/row values for selector, then clear the secondary spawn
 * list. When preserveIndexed == 0, also clear the indexed spawn list and
 * deactivate both fixed slots. Pool pointers/counts must already be bound.
 * Other record pools are refreshed by the prerequisite services, not zeroed. */
void Ovl11PrepareSpawnRecordPools(u16 selector, s32 preserveIndexed);

/* Clear id/kind/x/y/z and active bit 0 in count consecutive 0x18-byte records.
 * Keep all other flags, +6 and +14. A nonpositive count does nothing. */
void Ovl11ClearSpawnRecords(Ovl11DispatchRecord *records, s32 count);

/* Select fixed slot 0 for kind 0x64, fixed slot 1 for 0x65, or indexed slot
 * for 0xE8/0x122/0x123/0x124. Other kinds are ignored. Only the low halfword of
 * kind is used. Indexed bounds are unchecked; index is ignored for fixed slots.
 * Store low-16 id/kind and three unscaled signed coordinate words, set active
 * bit 0, preserve other fields. The by-value SDK VECTOR includes an unused
 * fourth word; it is not a three-word aggregate ABI. */
void Ovl11SetSpawnRecord(s32 kind, VECTOR position, u16 id, s32 index);

/* Same slot selection. Copy three coordinate words and signed id even when
 * inactive, then return active bit 0 (0 or 1). Unsupported kind returns 0
 * without touching either output. Outputs must be valid for a supported kind. */
s32 Ovl11ReadSpawnRecord(u16 kind, s32 *position, s16 *id, s32 index);

#endif
