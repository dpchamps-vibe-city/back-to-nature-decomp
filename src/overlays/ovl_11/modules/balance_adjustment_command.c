/*
 * balance_adjustment_command — reconstructed translation unit.
 *
 * Adapt a literal or value-slot balance delta to the accounting service and
 * optionally publish its status.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_balance_adjustment_command.h"

s32 Ovl11AdjustBalanceCommand(s16 clampDebit, s16 statusSlot, s32 deltaOrSlot, s32 fromSlot) {
    s32 delta;
    s16 status;

    if (fromSlot != 0) {
        delta = gOvl11ValueSlots[deltaOrSlot];
    } else {
        delta = deltaOrSlot;
    }
    status = Ovl11AdjustSpendableBalance(delta, clampDebit);
    if (statusSlot != -1) {
        gOvl11ValueSlots[statusSlot] = status;
    }
    return status == 0;
}
