#ifndef OVL11_ACTION_RESULT_OVERRIDE_COMMAND_H
#define OVL11_ACTION_RESULT_OVERRIDE_COMMAND_H

#include "common.h"

/* ovl_11 must be resident; the five-entry override table must be initialized.
 * Resolve actionOrSlot through the shared value bank iff fromSlot != 0;
 * that signed slot index is unchecked and the resolved value stays s32.
 * Scan signed ID/result halfword pairs in order. The FIRST entry whose ID
 * matches the full target OR is -1 is written (both ID and result), returning
 * 1. A free entry can precede a later matching ID. Return 0 unchanged when all
 * five entries hold other IDs. Writes narrow the target to s16; comparison
 * does not. No allocation outside this fixed table. Original: ovl_11:800ED760. */
s32 Ovl11SetActionResultOverrideCommand(s16 actionOrSlot, s16 result, s32 fromSlot);

#endif
