#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D740C(u16 arg0);

s32 ovl_11_func_80116878(StructD548 *arg0) {
    s16 threshold;

    threshold = ovl_11_func_800D740C((u16) arg0->data[arg0->index]);
    return D_80071A60 >= threshold;
}
