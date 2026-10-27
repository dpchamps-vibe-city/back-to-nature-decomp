/* Read-only purchase preflight (ovl_11:8011D06C–8011D084).
 * This does not reserve funds: transaction controllers perform the later
 * debit through Ovl11AdjustSpendableBalance. It is independent of the
 * neighboring quantity-input and interaction-step services.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_balance_accounting.h"

s32 Ovl11HasSpendableBalance(s32 amount) {
    return gSpendableBalance >= amount;
}
