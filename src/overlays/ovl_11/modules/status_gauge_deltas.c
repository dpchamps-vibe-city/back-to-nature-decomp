/* Signed-delta status services, original ovl_11:800D622C-800D6488.
 * The paired gauge setters share +12/current, +14/limit and +16/secondary
 * in gPrimaryObjectBase. The intervening balance adapter uses the same
 * success/result convention. All three return 1; none validates state setup.
 *
 * Intermediate halfword stores and signed re-reads are intentional: clamping
 * follows s16 narrowing, not an unbounded mathematical addition. The counter
 * at +58 instead uses signed word arithmetic. Do not merge these contracts.
 * Externally used entry: ovl11_status_gauge_deltas.h. */

#include "common.h"
#include "game_types.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_status_gauge_deltas.h"

s32 Ovl11ApplyStatusDelta(s8 gaugeDelta, s8 secondaryDelta, s16 counterDelta, s32 unused) {
    char *base = (char *)&gPrimaryObjectBase;
    s32 gaugeSum;
    s32 nonnegativeGauge;
    s16 limit;
    s32 gaugeValue;
    s32 secondarySum;
    s32 cappedSecondary;
    s32 nonnegativeSecondary;
    s32 nonnegativeCounter;
    s32 cappedCounter;
    char *state;

    if (gaugeDelta != 0) {
        gaugeSum = *(u16 *)(base + 0x12) + gaugeDelta;
        *(u16 *)(base + 0x12) = gaugeSum;
        if ((s16)gaugeSum < 0) {
            nonnegativeGauge = 0;
        } else {
            nonnegativeGauge = gaugeSum;
        }
        *(u16 *)(base + 0x12) = nonnegativeGauge;
        limit = *(s16 *)(base + 0x14);
        if ((s16)nonnegativeGauge < limit) {
            gaugeValue = nonnegativeGauge;
        } else {
            gaugeValue = limit;
        }
        *(u16 *)(base + 0x12) = gaugeValue;
    }
    if ((secondaryDelta < 0) || ((secondaryDelta > 0) && !(*(u16 *)(base + 0x36) & 8))) {
        secondarySum = *(u16 *)(base + 0x16) + secondaryDelta;
        *(u16 *)(base + 0x16) = secondarySum;
        if ((s16)secondarySum < 0) {
            nonnegativeSecondary = 0;
        } else {
            nonnegativeSecondary = secondarySum;
        }
        *(u16 *)(base + 0x16) = nonnegativeSecondary;
        cappedSecondary = nonnegativeSecondary;
        if ((s16)cappedSecondary >= 0x65) {
            cappedSecondary = 0x64;
        }
        *(u16 *)(base + 0x16) = cappedSecondary;
        if ((*(s32 *)((char *)&gPrimaryObjectBase - 0xD08) == 6) && ((s16)cappedSecondary >= 0x64)) {
            *(u16 *)(base + 0x16) = 0x63;
        }
    }
    if (counterDelta != 0) {
        state = (char *)&gPrimaryObjectBase - 0x51C8;
        *(s32 *)(state + 0x5220) = *(s32 *)(state + 0x5220) + counterDelta;
        nonnegativeCounter = *(s32 *)(base + 0x58);
        if (nonnegativeCounter < 0) {
            nonnegativeCounter = 0;
        }
        *(s32 *)(state + 0x5220) = nonnegativeCounter;
        cappedCounter = *(s32 *)(base + 0x58);
        if (cappedCounter > 0xFFFF) {
            cappedCounter = 0xFFFF;
        }
        *(s32 *)(state + 0x5220) = cappedCounter;
    }
    return 1;
}

/* No known callers. Optional output receives the balance service's result,
 * not this adapter's constant success return. Keep signed-byte input widths. */
s32 Ovl11AdjustBalanceAndReport(s8 delta, s8 clamp, s32 unused, Ovl11BalanceAdjustmentResult *statusOut) {
    s32 balanceResult;
    s32 success;
    balanceResult = Ovl11AdjustSpendableBalance(delta, clamp);
    if (((s32)statusOut) == 0) {
        success = 1;
    } else {
        statusOut->result = balanceResult;
        success = 1;
    }
    return success;
}

/* No known callers. Adjust/clamp the limit, mirror it to the current gauge,
 * and count the nonzero limit adjustment (even when clamping cancels it).
 * Secondary changes here have neither the +36 bit-3 gate nor the hour-6 cap. */
s32 Ovl11ApplyStatusLimitDelta(s8 limitDelta, s8 secondaryDelta) {
    char *base = (char *)&gPrimaryObjectBase;
    char *state;
    s32 sum;
    s32 nonnegative;
    s32 value;

    if (limitDelta != 0) {
        sum = *(u16 *)(base + 0x14) + limitDelta;
        *(u16 *)(base + 0x14) = sum;
        if ((s16)sum < 0) {
            nonnegative = 0;
        } else {
            nonnegative = sum;
        }
        *(u16 *)(base + 0x14) = nonnegative;
        value = nonnegative;
        if ((s16)value >= 0x100) {
            value = 0xFF;
        }
        *(u16 *)(base + 0x14) = value;
        *(u16 *)(base + 0x12) = value;
        state = base - 0x51C8;
        *(u16 *)(state + 0x4A84) = *(u16 *)(state + 0x4A84) + 1;
    }
    if (secondaryDelta != 0) {
        sum = *(u16 *)(base + 0x16) + secondaryDelta;
        *(u16 *)(base + 0x16) = sum;
        if ((s16)sum < 0) {
            nonnegative = 0;
        } else {
            nonnegative = sum;
        }
        *(u16 *)(base + 0x16) = nonnegative;
        if ((s16)nonnegative >= 0x65) {
            nonnegative = 0x64;
        }
        *(u16 *)(base + 0x16) = nonnegative;
    }
    return 1;
}
