#include "common.h"
#include "random.h"

s32 ovl_17_func_800B9E34(s32 arg0) {
    u16 temp = (arg0 * 100) / 127;

    return Rand(0x64) < temp;
}
