#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

s32 ovl_11_func_800E8C14(s16 arg0, s16 arg1, s32 arg2) {
    s32 value;
    s32 check_result;
    s32 result;
    s32 index;

    if (arg2 != 0) {
        index = gOvl11ValueSlots[arg0];
    } else {
        index = arg0;
    }
    value = ((struct Ovl11E8C14State *)D_8006C838)->value;
    if (value == 0) {
        return 0;
    }
    check_result = Ovl11GetRecordItemId(&((struct Ovl11E8C14State *)D_8006C838)->value);
    if (check_result != 0) {
        value = check_result;
    }
    result = Ovl11GetItemPackedProperty(value, index);
    if (result < 7) {
        if (result >= 0) {
            gOvl11ValueSlots[arg1] = result + 1;
            return 1;
        }
    }
    gOvl11ValueSlots[arg1] = 0;
    return 1;
}
