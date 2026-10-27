#ifndef OVL11_HELD_ITEM_REPLACEMENT_H
#define OVL11_HELD_ITEM_REPLACEMENT_H

#include "common.h"
#include "game_types.h"

/* Replace the six-byte held item at object+8A with {low16(itemId), 0, 1},
 * staged through +F0. Save the old record in gOvl11SavedHeldItem, remove its
 * sprite slot if nonzero, install the new slot and clear object flag 4000.
 * The full-word itemId ABI is intentional; only the stored ID is narrowed.
 * presentation 0/1 selects state 30 on the global primary object, 2 selects
 * state 31 with sound 4A, and 3 selects state 4C with sound 4B. Item flags
 * 1200/1600 override that choice with 2/3 (the latter wins). Other choices
 * leave the staged record installed but do not copy it into the held slot.
 * Uses unchecked item-table indices and shared snapshot/cache state. */
void Ovl11ReplaceHeldItem(Ovl11C7010Object *object, s32 itemId, s32 presentation);

/* If item flag 100 is set, refresh its placed record at grid offset 2 using
 * the object's +FC tag. Then remove its sprite slot and clear the held
 * triplet. No empty-ID guard is applied here; callers supply a valid item. */
void Ovl11ReleaseHeldItem(Ovl11C7010Object *object);

#endif
