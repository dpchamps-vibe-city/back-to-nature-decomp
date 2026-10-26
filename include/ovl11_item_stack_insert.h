#ifndef OVL11_ITEM_STACK_INSERT_H
#define OVL11_ITEM_STACK_INSERT_H

#include "common.h"

/* Resident ovl_11 and initialized item state required. Records are three
 * halfwords {id, variant, quantity}; id 0 denotes an empty record. Normal
 * stacks hold up to 99. The saved exponent determines the number of slots as
 * 2^(exponent+1); no bounds check or corrupt-state clamp is performed. */

/* Free quantity for id in the held record followed by the normal slots,
 * narrowed to s16. mode==0 first runs a special availability predicate and
 * returns 99 immediately when it succeeds; any other mode skips that test.
 * Overflow records are not counted. */
s16 Ovl11GetItemStackCapacity(s16 id, s32 mode);

/* Return the unplaced quantity. mode==1 bypasses the total-capacity check,
 * permitting partial insertion. Otherwise, insufficient normal capacity
 * routes the ENTIRE amount to the first empty one of 64 overflow records;
 * that fallback does not cap the quantity at 99. No empty overflow record
 * leaves amount unchanged. With sufficient/bypassed capacity, fill the held
 * record then normal slots, always writing variant 0. ID bookkeeping runs
 * when the persistent fresh-slot flag is set; it is not reset per slot.
 * Callers supply valid nonzero IDs and nonnegative s16 amounts. */
s32 Ovl11InsertItemStack(s16 id, s16 amount, s32 mode);

#endif
