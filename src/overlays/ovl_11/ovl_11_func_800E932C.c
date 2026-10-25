#include "common.h"
#include "game_types.h"

/* This call forwards the full word; the callee stores its low halfword. */
void ovl_11_func_800CBB9C();

s32 ovl_11_func_800E932C(s16 arg0, s16 arg1, s32 arg2) {
    s32 value;
    Ovl11C7010Object *object = (Ovl11C7010Object *)&D_80071A00;

    if (arg1 != 0) {
        value = gOvl11ValueSlots[arg0];
    } else {
        value = arg0;
    }
    ovl_11_func_800CBB9C(object, value, arg2);
    return 1;
}
