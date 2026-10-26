#ifndef OVL11_ITEM_REQUEST_ACTIONS_H
#define OVL11_ITEM_REQUEST_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Resolve the selected interaction hit into {id,state,count}, commit a
 * nonzero id to the primary-object request, and clear object flag 0x200000.
 * Return 1 on commitment, otherwise 0. Resolution consumes the shared hit
 * latch even when no request is committed, and writes object +0x50/+0x54.
 * The object is the primary-object view, not an arbitrary request buffer. */
s32 Ovl11CommitSelectedItemRequest(Ovl11C7010Object *object);

/* Ignore input unless pressed has bit 0x40. With no action flags 0x18000000,
 * require hit category 0x2000 and try the commit above; otherwise delegate to
 * the item-action gate in mode 0. Return 1 if handled, 0 otherwise.
 * Shared request/hit state makes both operations non-reentrant. */
s32 Ovl11DispatchItemRequestInput(Ovl11C7010Object *object, u16 pressed);

#endif
