#ifndef OVL11_SPAWN_EFFECT_CONSUME_H
#define OVL11_SPAWN_EFFECT_CONSUME_H

#include "common.h"
#include "game_types.h"

/* Cell-action gates on a live spawn-effect record. A successful gate clears
 * bit 0 of the record's +4 flags, passes the ID at +2 and the four-word
 * position at +8 to the sprite-effect pool, and returns 1. A rejected gate
 * returns 0. This does not report whether the effect pool had room. */

/* Require a nonzero mapped item ID, far-state mode !=0x13 and the record
 * ID's 0x8000 item-attribute predicate. */
s32 Ovl11ConsumeSpawnEffect(Ovl11SpawnEffectRecord *record);

/* Require mapped ID's 0x2000 predicate and both attribute budgets. Comparisons
 * cast the signed halfword budgets to u32: negative values retain that
 * historical unsigned behavior, rather than being clamped or rejected. */
s32 Ovl11ConsumeSpawnEffectWithBudgets(Ovl11SpawnEffectRecord *record,
                                      s16 attribute2Budget, s16 attribute0Budget);

/* As above using 0x4000. Clears work flag 0x20 after ID mapping; ID 0x174
 * sets it back, posts value 5 to F0FB0 and returns 0 without consumption.
 * Successful selected IDs also increment the capped halfword service. */
s32 Ovl11ConsumeTieredSpawnEffect(Ovl11SpawnEffectRecord *record,
                                 s16 attribute2Budget, s16 attribute0Budget);

#endif
