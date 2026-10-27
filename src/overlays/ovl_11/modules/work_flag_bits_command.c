/*
 * work_flag_bits_command — primary work-area flag mask operations.
 *
 * Reads use absolute field symbols; writes retain the work-base struct view.
 * Both name the same storage. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_work_flag_bits_command.h"

s32 Ovl11WorkFlagBitsCommand(s16 testOnly, s16 setBits, s32 mask, s32 auxiliary) {
    if (auxiliary == 0) {
        if (testOnly != 0) {
            return (gPrimaryObjectFlags & mask) != 0;
        }
        if (setBits != 0) {
            struct Ovl11ResetState *view = (struct Ovl11ResetState *)&D_8006C838;
            view->field_5234 |= mask;
        } else {
            struct Ovl11ResetState *view = (struct Ovl11ResetState *)&D_8006C838;
            view->field_5234 &= ~mask;
        }
    } else {
        if (testOnly != 0) {
            return (gPrimaryObjectAuxFlags & mask) != 0;
        }
        if (setBits != 0) {
            struct Ovl11ResetState *view = (struct Ovl11ResetState *)&D_8006C838;
            view->field_51FE |= mask;
        } else {
            struct Ovl11ResetState *view = (struct Ovl11ResetState *)&D_8006C838;
            view->field_51FE &= ~mask;
        }
    }
    return 1;
}
