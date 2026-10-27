/* Begin/cancel/poll scripted primary-object input, optionally facing position.
 * Membership/boundaries: notes/file-groupings.md. */

#include "common.h"
#include "psyq/abs.h"
#include "ovl11_primary_object_input_override.h"
#include "ovl11_object_state_selection_command.h"
#include "ovl11_primary_input_override_command.h"

s32 Ovl11PrimaryInputOverrideCommand(s16 mode, s16 ticks) {
    s16 direction;
    s32 magnitudeX;
    s32 duration;
    s32 result;
    s32 magnitudeZ;

    switch (mode) {
    case 2:
        magnitudeX = abs(gObjectWorkPosition[0]);
        magnitudeZ = abs(gObjectWorkPosition[2]);
        if (magnitudeZ < magnitudeX) {
            direction = 1;
            if (gObjectWorkPosition[0] > 0) {
                direction = 3;
            }
        } else {
            direction = (gObjectWorkPosition[2] > 0) * 2;
        }
        Ovl11SelectObjectStateOrDirectionCommand(direction, 0, 0, 0);
        /* fallthrough */
    case 1:
        if (ticks != 0) {
            duration = ticks / 2;
        } else {
            duration = 0xC;
        }
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 |= 0x800000;
        Ovl11BeginPrimaryObjectInputOverride((s16) duration);
        result = 1;
        break;
    case -1:
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_51F6 = 0;
        ((struct struct_8006C838_800E8F78 *) D_8006C838)->field_5234 &= 0xFF7FFFF7;
        result = 1;
        break;
    default:
        result = 0;
        if (!(gPrimaryObjectFlags & 8)) {
            result = 1;
        }
        break;
    }
    return result;
}
