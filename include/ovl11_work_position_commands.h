#ifndef OVL11_WORK_POSITION_COMMANDS_H
#define OVL11_WORK_POSITION_COMMANDS_H

#include "common.h"

/* Resident ovl_11, initialized primary-object/collision state and value slots
 * required. These keep the four-operand object-script callback ABI. Position
 * words retain the caller's raw units; no conversion or slot bounds checks. */

/* mode 0 writes {x,y,z}, sign-extending the s16 y operand. mode 1 treats
 * y/x/z as value-slot indices. mode 2 commits the existing position through
 * the object service and resets its selected state to 0. Other modes leave
 * position unchanged. EVERY mode then resets the primary-object request,
 * copies collision state from collision work, and returns 1. */
s32 Ovl11ApplyWorkPositionCommand(s16 mode, s16 y, s32 x, s32 z);

/* operandFlags bit 0 resolves boundArg through value slots; bit 1 resolves
 * directionArg likewise. Direction 0/1 adopts a bound >= current z/x;
 * 2/3 adopts a bound <= current z/x. Equality succeeds. Return 1 on adoption
 * and clear primary flag 2; rejection returns 0 and leaves that flag set.
 * Rejection writes both direction words (+522C/+5230) to 0x4000/0x8000/
 * 0x1000/0x2000 respectively; an odd rejectionMode also ORs 0x40 into both.
 * Invalid directions leave those words unchanged except for that optional
 * OR, leave flag 2 set, and return 0. No y-coordinate update occurs. */
s32 Ovl11UpdateWorkPositionBound(s16 rejectionMode, s16 directionArg, s32 boundArg, s32 operandFlags);

#endif
