#include "common.h"

void ovl_19_func_800B998C(void);
void ovl_19_func_800BA770(void);

void ovl_19_func_800B87C4(void) {
    void (*temp_a0)(void);

    if (D_800BF4C0[3] == 7) {
        D_800BF4C0[3] = 0;
        D_800BF4C0[2] = 3;
        return;
    }
    temp_a0 = D_800BCEC8[D_800BF4C0[3]];
    temp_a0();
    ovl_19_func_800B998C();
    ovl_19_func_800BA770();
}
