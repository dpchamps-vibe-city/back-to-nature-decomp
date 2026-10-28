/*
 * record_tier_update — reconstructed translation unit.
 *
 * Apply divisor-scaled tier values and Q12 deltas to a caller-supplied
 * record, with the two signed-halfword lookup tables it consumes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl25_fixed_point_tier_lookup.h"
#include "ovl25_record_tier_update.h"
#include "ovl25_record_state_timer.h"

/* Preserve each signed-halfword truncation, including every decay iteration.
 * The kind at +0 is distinct from the state selector at +2. */
void Ovl25AccumulateRecordTierValue(s16 *record) {
    s16 kind;
    s16 decaySteps;
    s32 step;
    s16 increment;

    increment = (s16)(Ovl25LookupTierValue(*(s32 *)((u8 *)record + 0xC)) /
                      D_800BFE44_view.tierUpdateDivisor);
    kind = record[0];
    if (kind == 5) {
        decaySteps = D_800BFE44_view.unkC / 30;
        increment = D_800BFE44_view.kind5TierAmount / 30;
        for (step = 0; step < decaySteps; step++) {
            increment = (increment * (100 - D_800BFE44_view.kind5TierDecayPercent)) / 100;
        }
        if (increment <= 0) {
            increment = 1;
        }
        *(s16 *)((u8 *)record + 4) = 1;
    } else if (kind == 4) {
        increment = D_800BFE44_view.kind4TierAmount / 30;
        *(s16 *)((u8 *)record + 4) = 1;
    } else if (kind != 0) {
        increment = (increment * D_800BFE44_view.otherKindTierPercent) / 100;
    }
    record[8] += increment;
    if (*(s16 *)((u8 *)record + 4) <= 0) {
        Ovl25SetRecordState(record, 2, 0);
    }
    if ((u32)((u16)record[0] - 4) >= 2U) {
        *(s32 *)((u8 *)record + 0xC) = *(s32 *)((u8 *)record + 0xC) +
            ((s32)(Ovl25LookupTierDelta(*(s32 *)((u8 *)record + 0xC)) << 12) /
             D_800BFE44_view.tierUpdateDivisor);
    }
}

/* Keep the bias/shift pair in both lookups: division alone loses the
 * signed-16-bit integer-part truncation. The value lookup also serves the
 * scene input handler; the delta lookup is used only by the updater above. */

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
