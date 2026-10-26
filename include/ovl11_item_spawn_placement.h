#ifndef OVL11_ITEM_SPAWN_PLACEMENT_H
#define OVL11_ITEM_SPAWN_PLACEMENT_H

#include "common.h"
#include "game_types.h"

/* Area/time item placement. Calls the two area helpers first, then the
 * shared 17-rule placement table when the work +E4C8 mode is zero. Allocation
 * failures are skipped. Successful ordinary rules set the signed 16-bit
 * rule mask; IDs 0x163/0x166 do not. Preserve its signed promotion for rule
 * 16; this is not a 32-bit mask. In area 1, stage 2 can add three ID-0x154
 * items at hours [6,9), and a latched window can add ID 0x15B at [18,23)
 * unless stage 12. No general once-per-frame or duplicate-spawn guard exists.
 * Random calls and allocator order are observable shared-state effects. */
void Ovl11SpawnTimedItemPlacements(void);

/* Attempt ID 0x156 when mode==1, otherwise 0x157, at the supplied native
 * X/Y/Z coordinates. The 16-byte by-value record is part of the existing ABI;
 * its fourth word is unused. Work mode +E4C8 must be zero, the 1-in-50 draw
 * must pass, and +44BA must not be 3. Mode 1 copies the work +5200 facing;
 * other modes randomly choose 0/3. Returns no success indication. */
void Ovl11TrySpawnPositionedItem(Ovl11ItemSpawnPosition position, s32 mode);

/* If either indexed-record query succeeds and the work mode gates allow,
 * a 1-in-10 draw ORs bit 0x800000 into work +44F8. Never clears the bit;
 * placement later observes it together with bit 0x1000000. */
void Ovl11RollSpawnWindowLatch(void);

#endif
