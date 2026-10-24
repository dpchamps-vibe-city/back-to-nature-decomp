#include "common.h"

void ovl_23_func_800B9310(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4 at 0x800226A4, named by the overlay's symbol map. */
s32 GetVal8005E5B4(void);

void ovl_23_func_800B85FC(void) {
    ovl_23_func_800B9310();
    func_8002261C(4, 0x2C);
    if (GetVal8005E5B4() == 2) {
        Ovl23_D_800BF87C_bounds.unk0 = 4;
    }
}
