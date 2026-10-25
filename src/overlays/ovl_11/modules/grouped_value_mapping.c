/*
 * Paired maps for a signed-halfword value code. Values 1..25 form five
 * groups of five; 26..29 form four special groups. Action-record callers
 * use the group to select an action and add their own five-value offset
 * to the mapped code. Neither function reads or changes global state.
 *
 * Original entries: ovl_11:800CB020 and 800CB0B0. Membership and the
 * deliberately conservative boundaries are recorded in file-groupings.md.
 */

#include "common.h"
#include "ovl11_grouped_value_mapping.h"

s16 Ovl11MapGroupedValueCode(s16 value) {
    s32 group;
    s32 specialIndex;
    s32 firstValue;
    s16 baseCode;

    specialIndex = value - 0x1A;
    if ((u32) ((value - 1) & 0xFFFF) < 0x19U) {
        group = (value - 1) / 5;
        baseCode = (group * 0x14) + 0x48;
        firstValue = (group * 5) + 1;
        return (s16) (baseCode + (value - firstValue));
    }
    if ((u32) (specialIndex & 0xFFFF) < 4U) {
        return (s16) ((specialIndex * 4) + 0xB6);
    }
    return 0;
}

s16 Ovl11GetGroupedValueIndex(s16 value) {
    if (value >= 1 && value <= 25) {
        return (value - 1) / 5;
    }
    if (value >= 26 && value <= 29) {
        return (s16) (value - 26);
    }
    return 0;
}
