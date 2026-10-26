/*
 * Item flag/status/type queries and action dispatch, ovl_11:800D5750-800D5960.
 * Every entry indexes the shared 0x28-byte ItemData table. No table storage
 * is owned here. The signed flag view used by queries and unsigned flag
 * view used by dispatch deliberately preserve the original access widths.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_item_table_queries.h"

s32 Ovl11ClassifyItemFlags(s16 itemIndex) {
    s32 flags = gItemDataTable[itemIndex].u0.field_00;
    s32 result;

    result = 2;
    if (flags & 0x4000) {
        result = 0;
    } else if (flags & 0x8100) {
        result = 1;
    }
    return result;
}

s32 Ovl11ClassifyItemStatus(s16 itemIndex) {
    s32 status = gItemDataTable[itemIndex].field_18;
    s32 result;

    result = 3;
    if (status & 0x10000000) {
        result = 0;
    } else if (status & 0x20000000) {
        result = 1;
    } else if (status & 0x40000000) {
        result = 2;
    }
    return result;
}

/* Named zero operands preserve comparison instructions rather than the
 * compiler's alternate sign-bit/shift forms in these three predicates. */
s32 Ovl11IsItemStatusNegative(s16 itemIndex) {
    s32 zero = 0;

    return gItemDataTable[itemIndex].field_18 < zero;
}

s8 Ovl11GetItemType(s16 itemIndex) {
    return gItemDataTable[itemIndex].type;
}

s32 Ovl11ItemHasFlag40(s16 itemIndex) {
    s32 flags = gItemDataTable[itemIndex].u0.field_00;
    u32 zero = 0;

    return (flags & 0x40) > zero;
}

s32 Ovl11ItemHasFlag8100(s16 itemIndex) {
    s32 flags = gItemDataTable[itemIndex].u0.field_00;
    u32 zero = 0;

    return (flags & 0x8100) > zero;
}

s32 Ovl11InvokeItemAction(void *itemRecord) {
    ItemData *entry;
    ItemAction *action;
    s32 firstArgument;

    entry = &gItemDataTable[*(s16 *)itemRecord];
    action = &entry->action;

    if (entry->u0.flags & 0x800) {
        firstArgument = *(s8 *)((char *)itemRecord + 2);
    } else {
        firstArgument = action->field_4;
    }

    if (action != 0 && action->field_0 != 0) {
        return action->field_0(firstArgument, action->field_5, action->field_6, 0);
    }
    return -1;
}
