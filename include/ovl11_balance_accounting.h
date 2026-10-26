#ifndef OVL11_BALANCE_ACCOUNTING_H
#define OVL11_BALANCE_ACCOUNTING_H

#include "common.h"

/* Add signed balance units to gSpendableBalance and the newest credit/debit
 * history pair. Returns 0 normally, 1 when the proposed balance is at least
 * 9,999,999 (stored as that cap), or 2 on insufficient balance.
 *
 * An insufficient debit leaves the balance unchanged when clampDebit == 0;
 * otherwise it consumes only the available balance. Debit history records
 * only the consumed amount. Credit history records the full requested delta,
 * even when the balance saturates. A zero delta returns 0 without any writes.
 * Inputs and history totals must stay within signed 32-bit arithmetic range;
 * this routine does not guard arithmetic overflow or repair invalid state. */
s32 Ovl11AdjustSpendableBalance(s32 delta, s32 clampDebit);

#endif
