/*
 * Object-script commands sharing the three-word work position. The position
 * setter/commit operation and directional-bound operation are complementary;
 * they are not owners of the larger, widely shared primary-object state.
 * Membership and conservative outer boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_work_position_commands.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_default_position.h"
#include "ovl11_object_state_selection_command.h"

s32 Ovl11ApplyWorkPositionCommand(s16 mode, s16 y, s32 x, s32 z) {
    s32 *position = gObjectWorkPosition;

    switch (mode) {
    case 0:
        position[1] = y;
        position[0] = x;
        position[2] = z;
        break;
    case 1:
        position[1] = gOvl11ValueSlots[y];
        position[0] = gOvl11ValueSlots[x];
        position[2] = gOvl11ValueSlots[z];
        break;
    case 2:
        Ovl11ReadDefaultPosition(position);
        Ovl11SelectObjectStateOrDirectionCommand(0, 0, 0, 0);
        break;
    }
    Ovl11ResetPrimaryObjectRequest();
    ((Ovl11CBE2CState *)D_8006C838)->collision =
        ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision;
    return 1;
}

s32 Ovl11UpdateWorkPositionBound(s16 rejectionMode, s16 directionArg, s32 boundArg, s32 operandFlags) {
    s32 direction;
    s32 bound;
    s32 adopted;
    s16 rejectionParity;
    char *work;
    s32 *position;

    bound = boundArg;
    if (operandFlags & 1) {
        bound = gOvl11ValueSlots[bound];
    }
    if (operandFlags & 2) {
        direction = gOvl11ValueSlots[directionArg];
    } else {
        direction = directionArg;
    }
    position = gObjectWorkPosition;
    work = (char *)(position - 0x14B2);
    adopted = 0;
    rejectionParity = rejectionMode % 2;
    *(s32 *)(work + 0x5234) |= 2;
    switch (direction) {
    case 0:
        if (!(bound < position[2])) {
            position[2] = bound;
            adopted = 1;
        } else {
            s32 reversal = 0x4000;
            *(s32 *)(work + 0x522C) = reversal;
            *(s32 *)(work + 0x5230) = reversal;
        }
        break;
    case 1:
        if (!(bound < position[0])) {
            position[0] = bound;
            adopted = 1;
        } else {
            s32 reversal = 0x8000;
            *(s32 *)(work + 0x522C) = reversal;
            *(s32 *)(work + 0x5230) = reversal;
        }
        break;
    case 2:
        if (!(position[2] < bound)) {
            position[2] = bound;
            adopted = 1;
        } else {
            s32 reversal = 0x1000;
            *(s32 *)(work + 0x522C) = reversal;
            *(s32 *)(work + 0x5230) = reversal;
        }
        break;
    case 3:
        if (!(position[0] < bound)) {
            position[0] = bound;
            adopted = 1;
        } else {
            s32 reversal = 0x2000;
            *(s32 *)(work + 0x522C) = reversal;
            *(s32 *)(work + 0x5230) = reversal;
        }
        break;
    }
    /* Keep the canonical work-area member view here: it denotes the same
     * storage while preserving the target's independent address formation. */
    if (adopted == 1) {
        ((struct struct_8006C838_800E8F78 *) &D_8006C838)->field_5234 &= ~2;
    } else if (rejectionParity != 0) {
        ((struct struct_8006C838_800E8F78 *) &D_8006C838)->field_522C |= 0x40;
        ((struct struct_8006C838_800E8F78 *) &D_8006C838)->field_5230 |= 0x40;
    }
    return adopted;
}
