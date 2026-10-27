#ifndef OVL11_PENDING_RECORD_ANIMATION_H
#define OVL11_PENDING_RECORD_ANIMATION_H

#include "common.h"

/* Consume work+51EA, optionally exporting its signed record id to resultSlot
 * (-1 skips export). animationPlusOne == 0 skips animation; otherwise use
 * animationPlusOne-1. Nonzero updateField writes state field 2C.
 * With requireFlag46 != 0, the actions and clearing wait for flag 46 == 1;
 * export still occurs before that test. Otherwise clear the pending id and
 * flag even when no record is pending. Return the gate result, also passed
 * to the scene-command flag helper. Slots and record ids are unchecked.
 */
s32 Ovl11CommitPendingRecordSelection(s16 resultSlot, s16 animationPlusOne,
                                     s32 updateField, s32 requireFlag46);

/* Resolve a literal record id, or a value-slot id when resolveSlot's low
 * halfword is nonzero. Classify direction toward gObjectWorkPosition.
 * recordSpace 1 writes only the F8-stride record's direction; other values
 * run the 1D4-stride record animation command and clear its flag 800.
 * Space 2 maps stored states 5..9 to animation 5, A to animation B/direction 0.
 * Other animations are narrowed to s16. Returns 1, with no range checks.
 */
s32 Ovl11OrientAndAnimateStateRecord(s16 recordOrSlot, s32 resolveSlot,
                                    s32 animation, s32 recordSpace);

#endif
