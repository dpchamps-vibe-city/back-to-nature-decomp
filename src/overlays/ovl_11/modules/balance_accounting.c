/*
 * balance_accounting — reconstructed translation unit.
 *
 * Spendable-balance adjustments with separate positive and negative history
 * totals. The signed delta is charged fully, rejected on insufficient funds,
 * or truncated to the available balance according to the caller flag; upper
 * saturation still records the requested credit.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_balance_accounting.h"

/* Word indices in the resident state, not independent storage. The balance
 * at +5224 is gSpendableBalance; +49C4/+49C8 are the newest history pair
 * shifted by Ovl11ClockShiftValueHistory. Separate branch-local bases retain
 * the original shared-high/address-low formation. */
enum {
    BALANCE_WORD = 0x1489,
    CREDIT_TOTAL_WORD = 0x1271,
    DEBIT_TOTAL_WORD = 0x1272
};

s32 Ovl11AdjustSpendableBalance(s32 delta, s32 clampDebit) {
    s32 *state;
    s32 *creditState;
    s32 *debitState;
    s32 proposedBalance;
    s32 debitTotal;
    s32 debitMagnitude;
    s32 status;

    if (delta == 0) {
        return 0;
    }

    state = (s32 *)&D_8006C838;
    status = 0;
    proposedBalance = state[BALANCE_WORD];
    proposedBalance += delta;
    if (proposedBalance < 0) {
        status = 2;
        if (clampDebit != 0) {
            state[BALANCE_WORD] = 0;
            delta = delta - proposedBalance;
        } else {
            delta = 0;
        }
    } else if (0x98967E < proposedBalance) {
        status = 1;
        state[BALANCE_WORD] = 0x98967F;
    } else {
        state[BALANCE_WORD] = proposedBalance;
    }

    /* Upper saturation limits the balance, not the credited history. */
    if (delta > 0) {
        creditState = (s32 *)&D_8006C838;
        creditState[CREDIT_TOTAL_WORD] += delta;
    } else {
        debitState = (s32 *)&D_8006C838;
        debitTotal = debitState[DEBIT_TOTAL_WORD];
        debitMagnitude = delta < 0 ? -delta : delta;
        debitTotal += debitMagnitude;
        debitState[DEBIT_TOTAL_WORD] = debitTotal;
    }
    return status;
}
