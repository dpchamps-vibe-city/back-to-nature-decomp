#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

s32 ovl_11_func_800E9914(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 result;

    result = 0;
    switch (arg0) {
    case 0:
        if (((struct Ovl11E9914State *)D_8006C838)->pending != 0) {
            if (Ovl11GetItemPackedProperty(((struct Ovl11E9914State *)D_8006C838)->pending, arg1) == 0) {
                result = 1;
            }
        }
        if (arg2 != -1) {
            gOvl11ValueSlots[arg2] = ((struct Ovl11E9914State *)D_8006C838)->pending;
        }
        ((struct Ovl11E9914State *)D_8006C838)->pending = 0;
        break;
    case 1:
        if (D_80070C88 & 0x20) {
            result = 1;
        }
        break;
    }
    return result;
}
