#include "common.h"
#include "game_types.h"

void ovl_11_func_800C7650(void);
s16 ovl_11_func_800CB0B0(s16 arg0);
/* 800C7464 passes the full addition; 800C70CC stores its low halfword. */
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8001FABC(s16 arg0);

s32 ovl_11_func_800C73FC(Ovl11C7010Object *arg0, u16 *arg1) {
    u16 temp_v1;

    temp_v1 = *arg1;
    if ((u32) (temp_v1 - 0x1A) < 4U) {
        if ((s16) temp_v1 != 0x1D) {
            ovl_11_func_800C7650();
        }
        arg0->unk6C |= 0x1800;
        ovl_11_func_800C70CC(arg0, ovl_11_func_800CB0B0((s16) *arg1) + 0x4D, (s32) arg0->unk38, -2);
        func_8001FABC((s16) (arg0->unk3C + 0xFFBD));
        return 1;
    }
    return 0;
}
