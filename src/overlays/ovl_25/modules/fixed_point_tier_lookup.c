/* Paired Q12-input tier lookups over private signed-halfword constants.
 * The shared caller duration-scales the first as a value and the second as
 * a Q12 delta. Membership/boundaries: notes/file-groupings.md.
 * Keep the bias/shift pair: replacing it with division alone loses the
 * signed-16-bit integer-part truncation. */

#include "ovl25_fixed_point_tier_lookup.h"

s32 Ovl25LookupTierValue(s32 inputQ12) {
    s32 tierValue;

    if (inputQ12 < 0) {
        inputQ12 += 0xFFF;
    }
    tierValue = (inputQ12 << 4) >> 16;
    if (tierValue >= 0x28) {
        tierValue = gOvl25TierValue40;
    } else if (tierValue >= 0x1E) {
        tierValue = gOvl25TierValue30;
    } else if (tierValue >= 0x14) {
        tierValue = gOvl25TierValue20;
    } else {
        tierValue = gOvl25TierValueBelow20;
    }
    return tierValue;
}

s32 Ovl25LookupTierDelta(s32 inputQ12) {
    s32 integerPart;

    if (inputQ12 < 0) {
        inputQ12 += 0xFFF;
    }
    integerPart = (inputQ12 << 4) >> 16;
    if (integerPart >= 0x28) {
        return gOvl25TierDelta40;
    }
    if (integerPart >= 0x1E) {
        return gOvl25TierDelta30;
    }
    if (integerPart >= 0x14) {
        return gOvl25TierDelta20;
    }
    if (integerPart >= 0xA) {
        return gOvl25TierDelta10;
    }
    return 0;
}
