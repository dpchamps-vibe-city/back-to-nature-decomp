#ifndef OVL11_STATE_FIELD_BROADCAST_COMMAND_H
#define OVL11_STATE_FIELD_BROADCAST_COMMAND_H

#include "common.h"

/* ovl_11 broadcast over event slots 1..36 (stride 0x1D4). Only records with
 * +0x1E bit 0x800 participate. Normally call Ovl11UpdateStateField(fieldId,
 * slot, operand, operation). For EITHER fieldId 0x1F or 0x20, split instead:
 * special-counter slots {3,7,9,14,20} receive field 0x1F/specialOperand;
 * all other participating slots receive field 0x20/operand.
 * Always returns 1; does not aggregate individual update results. Initialize
 * record/field storage and satisfy the selected field operation's unchecked
 * index/arithmetic constraints (ovl11_state_field_access.h).
 * Original identity: ovl_11:800ED4A8. */
s32 Ovl11BroadcastStateFieldCommand(s16 fieldId, s16 operation, s32 operand, s32 specialOperand);

#endif
