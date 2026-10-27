#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

s32 ovl_11_func_800E93CC(s16 arg0, s16 arg1) {
    ItemData *item;
    u32 flags;

    if (arg1 != 0) {
        arg1 = *(s16 *)&gOvl11ValueSlots[arg0];
    } else {
        arg1 = arg0;
    }
    if (arg1 == 0xA4) {
        return 1;
    }
    item = gItemDataTable;
    item += arg1;
    flags = item->u0.flags;
    flags &= 0x1C00;
    if (flags) {
        flags = Ovl11GetItemPackedProperty(arg1, 3);
        if ((u16)flags < 3) {
            return 1;
        }
    }
    return 0;
}
