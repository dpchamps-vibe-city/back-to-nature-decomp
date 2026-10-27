#ifndef OVL11_PURCHASE_TRANSACTIONS_H
#define OVL11_PURCHASE_TRANSACTIONS_H

#include "common.h"
#include "game_types.h"

/* Update-call state machines. Return 0 while pending, 1 on completion (which
 * includes cancellation); neither return value alone indicates a purchase.
 * Call repeatedly with stable arguments, starting at interaction step 0.
 * Shared interaction/message/selection state makes these non-reentrant.
 * Both set work-area +5234 bit 0x800000 and clear it on completion;
 * costs use the item's signed-halfword price units.
 *
 * Single-item mode 0 is the ordinary path, 1 updates the held-pair level and
 * progress display, and 2 performs the fixed spawn-record/entry reset path.
 * Other modes have no established caller contract. messageBank/promptId are
 * passed directly to RequestMessageWindow, without range checking. */
s32 Ovl11StepSingleItemPurchase(s16 catalogId, s16 messageBank, s16 promptId, s16 mode);

/* Each message points to two s16 values: {archive bank, entry ID}. selection
 * is the existing by-value selection descriptor, not a pointer. The quantity
 * prompt is capped at 5. Insufficient stock/funds retries without debiting;
 * the secondary-capacity warning DOES debit and insert before continuing to
 * nextMessage. Cancelling the quantity prompt returns to item selection;
 * cancelling item selection completes the handler. No arrays, descriptor indices or message IDs are bounds-checked. */
s32 Ovl11StepQuantityPurchase(s16 *startMessage, s16 *nextMessage, s16 *cancelMessage, Ovl11D8012D7B8 selection);

#endif
