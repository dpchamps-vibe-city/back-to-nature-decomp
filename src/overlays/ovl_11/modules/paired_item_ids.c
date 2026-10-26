/*
 * Paired item-ID ranges, ovl_11:800CE514..800CE550.
 * The upper range maps exactly onto the lower one through the conversion
 * between the predicates. Callers use these tests for item-action selection;
 * no gameplay category is inferred from the numeric relationship.
 */
#include "ovl11_paired_item_ids.h"

s32 Ovl11IsUpperPairedItemId(s16 itemId) {
    return (u32) (itemId - 0xED) < 6U;
}

s16 Ovl11MapToLowerPairedItemId(s16 itemId) {
    /* Halfword wrapping is intentional, including for unchecked inputs. */
    return itemId + 0xFF90;
}

s32 Ovl11IsLowerPairedItemId(s16 itemId) {
    return (u32) (itemId - 0x7D) < 6U;
}
