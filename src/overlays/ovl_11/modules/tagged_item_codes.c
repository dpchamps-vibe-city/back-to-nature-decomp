/*
 * Tagged-item command operands: export either a raw/resolved item code or
 * its one-based packed property into shared value slots. Tag 0xA4 embeds
 * the item ID in the following halfword; other tags can be item IDs directly.
 * See ovl11_tagged_item_codes.h for the differing empty/output-slot rules.
 */
#include "common.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_tagged_item_codes.h"

/* The fourth command word is supplied by 800EDAA4 but is not read here. */
s32 Ovl11StoreItemPropertyCode(s16 propertyOperand, s16 destinationSlot,
                               s32 useValueSlot, s32 reserved) {
    s32 item;
    s32 taggedItem;
    s32 property;
    s32 propertyIndex;

    if (useValueSlot != 0) {
        propertyIndex = gOvl11ValueSlots[propertyOperand];
    } else {
        propertyIndex = propertyOperand;
    }
    item = ((struct Ovl11TaggedItemState *)D_8006C838)->secondTag;
    if (item == 0) {
        return 0;
    }
    taggedItem = Ovl11GetRecordItemId(&((struct Ovl11TaggedItemState *)D_8006C838)->secondTag);
    if (taggedItem != 0) {
        item = taggedItem;
    }
    property = Ovl11GetItemPackedProperty(item, propertyIndex);
    if (property < 7) {
        if (property >= 0) {
            gOvl11ValueSlots[destinationSlot] = property + 1;
            return 1;
        }
    }
    gOvl11ValueSlots[destinationSlot] = 0;
    return 1;
}

s32 Ovl11StoreTaggedItemCode(s16 destinationSlot, s32 useSecondRecord, s32 resolveTag) {
    s16 code;
    s16 itemId;

    if ((useSecondRecord << 0x10) == 0) {
        struct Ovl11TaggedItemState *view = (struct Ovl11TaggedItemState *)&D_8006C838;
        code = view->firstTag;
        itemId = view->firstItemId;
    } else {
        struct Ovl11TaggedItemState *view = (struct Ovl11TaggedItemState *)&D_8006C838;
        code = view->secondTag;
        itemId = view->secondItemId;
    }
    if ((resolveTag != 0) && (code == 0xA4)) {
        code = itemId;
    }
    if (code == 0) {
        return 0;
    }
    if (destinationSlot != -1) {
        gOvl11ValueSlots[destinationSlot] = code;
    }
    return 1;
}
