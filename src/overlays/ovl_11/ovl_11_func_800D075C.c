#include "common.h"
#include "game_types.h"
#include "ovl11_row_placement.h"

void ovl_11_func_800D075C(s32 arg0, s16 arg1, s32 arg2) {
    if (arg2 == 1) {
        Ovl11PlaceRowZSlot(arg0, arg1);
    } else {
        Ovl11PlaceRowXSlot(arg0, arg1);
    }
}
