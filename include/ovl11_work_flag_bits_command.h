#ifndef OVL11_WORK_FLAG_BITS_COMMAND_H
#define OVL11_WORK_FLAG_BITS_COMMAND_H

#include "common.h"

/* ovl_11 command over the primary work area's shared flags. auxiliary == 0
 * selects the signed word at +0x5234; otherwise select u16 at +0x51FE.
 * testOnly != 0 returns whether ANY mask bit is set and changes nothing.
 * Otherwise setBits != 0 ORs the mask, zero clears it, and return 1.
 * Halfword writes truncate the mask to 16 bits, but halfword TESTS compare
 * the promoted u16 against the full s32 mask. No validation or synchronization
 * is performed; work storage must be initialized. Original: ovl_11:800E953C. */
s32 Ovl11WorkFlagBitsCommand(s16 testOnly, s16 setBits, s32 mask, s32 auxiliary);

#endif
