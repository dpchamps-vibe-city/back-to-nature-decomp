#include "common.h"

/* Engine setter SetVal8005E2E8, named by address in this overlay. */
void func_8001EF8C(s32 arg0);
s32 func_8001E4C0(void *arg0, s32 *arg1);
s32 func_8001E38C(void);

void ovl_19_func_800BB548(s16 *arg0) {
    s32 pos[4];

    pos[0] = arg0[0];
    pos[1] = arg0[1];
    pos[2] = arg0[2];
    pos[3] = arg0[3];
    func_8001EF8C(0xC000);
    if (func_8001E4C0(D_800BF700, pos)) {
        pos[1] = func_8001E38C();
    }
    arg0[0] = pos[0];
    arg0[1] = pos[1];
    arg0[2] = pos[2];
    arg0[3] = pos[3];
}
