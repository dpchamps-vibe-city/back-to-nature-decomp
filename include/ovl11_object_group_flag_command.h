#ifndef OVL11_OBJECT_GROUP_FLAG_COMMAND_H
#define OVL11_OBJECT_GROUP_FLAG_COMMAND_H

#include "common.h"

/* ovl_11:800EC064. group 0/4 selects the stage-mode-2/1 record; 1/2
 * selects the 20-entry B8-stride tagged pool, 3 the 10-entry B4-stride pool.
 * Nonzero indexFromSlot resolves recordIndex from a signed LOW halfword slot.
 * For pool groups, operation 0 instead finds the first record with flag 400;
 * no match returns 1 without further action. Other operations use the index.
 * operation 0/1 clears/sets flag 400 at record+34. 2 clears game flag 74 and
 * updates the stage-mode-1 record; 3 invokes item matching, clears work+91E0
 * bit 20000, sets flag 74 and updates that record. Other operations do nothing.
 * Return 0 for an unknown group, 1 otherwise, not asynchronous completion.
 * Requires initialized work/pools and valid unchecked indices/value slots. */
s32 Ovl11ApplyObjectGroupCommand(s16 group, s16 recordIndex, s32 operation,
                                 s32 indexFromSlot);

#endif
