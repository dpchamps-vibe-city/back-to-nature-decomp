#include "common.h"

void ovl_11_func_800CFE40(u8 *arg0);
void ovl_11_func_80107DD0(s16 *arg0);

void ovl_11_func_8010C2EC(u8 *arg0) {
    if (!(D_8006C844 & 0x800000)) {
        ovl_11_func_800CFE40(arg0);
        ovl_11_func_80107DD0((s16 *)(arg0 + 0xA8));
    }
}
