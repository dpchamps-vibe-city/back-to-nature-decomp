#ifndef OVL11_QUANTITY_PURCHASE_H
#define OVL11_QUANTITY_PURCHASE_H

#include "common.h"

/* Interaction 0x10 update callback. Drives quantity choice, nine-halfword name
 * entry, item insertion and list-selected record release. Uses the shared
 * primary selection/quantity prompt, message, text-entry and interaction-step
 * services; requires their initialized state and valid selected records.
 * Return 1 only at terminal step 24, otherwise 0. Non-reentrant.
 * Quantity purchases debit signed prompt(+A)*prompt(+8), then account quantity
 * through counter selector 2. Name acceptance copies all nine halfwords before
 * debiting the selected price. Confirmed record release credits 500 or 2000
 * (record flag 0x40) then clears that 0xB4-byte record. No rollback is added.
 * Each call clears work+522C/+5230 and sets bit 0x800000 at +5234; terminal
 * completion clears that bit. Message/portrait/selection effects persist in
 * shared engine state. Private name text overlaps its record at +4. */
s32 Ovl11UpdateQuantityPurchaseInteraction(void);

#endif
