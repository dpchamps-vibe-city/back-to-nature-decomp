#include "common.h"
#include "random.h"

void ovl_11_func_800DA454(u16 *arg0, s32 arg1) {
    if (arg1 != 0) {
        if (*arg0 == 0x3F) {
            if (Rand(arg1 & 0xFFFF) == 0) {
                *arg0 = 0x175;
            }
        }
    }
}
