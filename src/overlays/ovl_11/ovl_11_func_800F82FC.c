#include "common.h"

s32 ovl_11_func_800F8404(s32 arg0);

s16 *ovl_11_func_800F82FC(s32 column, s32 row) {
    Ovl11ItemState *base;
    Ovl11ItemState *selected;
    Ovl11ItemState *start;
    s32 offset;
    s32 index;
    s32 rowOffset;

    rowOffset = row * 8 - 9;
    index = rowOffset + column;
    if (gOvl11InteractionPhase == 0) {
        base = (Ovl11ItemState *)&D_80071A84;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 1;
    } else {
        base = &Ovl11Item_80071A8A;
        offset = column << 1;
        offset += column;
        offset <<= 1;
        start = base + 8;
    }
    do {
        selected = (Ovl11ItemState *)(offset + (u32)start);
    } while (0);
    offset = ovl_11_func_800F8404(column);
    switch (offset) {
    case 0:
        return (s16 *)base;
    case 1:
        return (s16 *)selected;
    default:
        switch (gOvl11InteractionPhase) {
        case 0:
            return (s16 *)((Ovl11ItemState *)&gItemOverflowSlotBase + index);
        default:
            return (s16 *)(D_80071042 + index);
        case 2:
            return (s16 *)(D_80070D42_slots[0] + index);
        }
    }
}
