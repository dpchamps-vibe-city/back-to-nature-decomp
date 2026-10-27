#include "common.h"
#include "game_types.h"
#include "ovl11_held_item_replacement.h"

s32 ovl_11_func_800E932C(s16 arg0, s16 arg1, s32 arg2) {
    s32 value;
    Ovl11C7010Object *object = (Ovl11C7010Object *)&gPrimaryObjectBase;

    if (arg1 != 0) {
        value = gOvl11ValueSlots[arg0];
    } else {
        value = arg0;
    }
    Ovl11ReplaceHeldItem(object, value, arg2);
    return 1;
}
