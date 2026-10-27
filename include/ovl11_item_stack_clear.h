#ifndef OVL11_ITEM_STACK_CLEAR_H
#define OVL11_ITEM_STACK_CLEAR_H

#include "common.h"

/* Zero all three halfwords of an item-stack entry (ID, detail, count).
 * Used after transfers and exhaustion. No validity/liveness check or
 * allocation side effects; entry must point to six writable aligned bytes. */
void Ovl11ClearItemStackEntry(s16 *entry);

#endif
