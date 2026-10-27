#ifndef OVL11_BALANCE_ADJUSTMENT_COMMAND_H
#define OVL11_BALANCE_ADJUSTMENT_COMMAND_H

#include "common.h"

/* ovl_11 command adapter for Ovl11AdjustSpendableBalance. A nonzero fromSlot
 * reads the full signed delta from gOvl11ValueSlots[deltaOrSlot]; otherwise
 * deltaOrSlot is the literal delta, in balance units. clampDebit is forwarded
 * unchanged. See ovl11_balance_accounting.h for cap/debit/history effects.
 *
 * Store the accounting status (0 normal, 1 cap, 2 insufficient) in statusSlot
 * unless it is -1; return 1 only for status 0. Thus a capped credit may change
 * balance/history while this command returns 0. Slot indices are unchecked;
 * the delta is read before publishing status, so input/output may alias.
 * Original identity: ovl_11:800E7CCC. */
s32 Ovl11AdjustBalanceCommand(s16 clampDebit, s16 statusSlot, s32 deltaOrSlot, s32 fromSlot);

#endif
