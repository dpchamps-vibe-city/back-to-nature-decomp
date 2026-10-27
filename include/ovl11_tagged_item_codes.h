#ifndef OVL11_TAGGED_ITEM_CODES_H
#define OVL11_TAGGED_ITEM_CODES_H

#include "common.h"

/* Shared work-area tag/item pairs live at +524C and +5252. Tag 0xA4
 * denotes an item ID in the next signed halfword. Other nonzero tags are
 * used directly as codes. These commands only write gOvl11ValueSlots;
 * neither changes the tagged records. All slot/property indices are unchecked.
 */

/* Read the SECOND record. Empty tag returns 0 without touching output.
 * A nonzero embedded item ID replaces tag A4 (an embedded zero does not).
 * useValueSlot resolves propertyOperand via the value slots when nonzero;
 * the packed-property lookup narrows that result to s16. Results 0..6 store
 * as 1..7; all other results store zero. Returns 1 after either store.
 * destinationSlot has NO -1 bypass. The original caller supplies a fourth,
 * unused command word; reserved preserves that public four-word contract. */
s32 Ovl11StoreItemPropertyCode(s16 propertyOperand, s16 destinationSlot,
                               s32 useValueSlot, s32 reserved);

/* useSecondRecord's low halfword chooses first (zero) or second (nonzero).
 * resolveTag != 0 substitutes the following item ID for tag A4, INCLUDING
 * zero. A zero selected/resolved code returns 0 with output unchanged;
 * otherwise writes the signed code and returns 1. destinationSlot == -1
 * suppresses only the store, not the record test or success result. */
s32 Ovl11StoreTaggedItemCode(s16 destinationSlot, s32 useSecondRecord, s32 resolveTag);

#endif
