#include "common.h"
#include "game_types.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800B9798(s16 arg0, s16 arg1);
void ovl_21_func_800BA4C0(void);
s32 func_80013394(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* Overlay names for GetVal8005E5B4 and GetVal8005E5B8 in the executable. */
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_21_func_800B859C(void) {
    s32 ready;
    s32 state;

    ovl_21_func_800B8320();
    ovl_21_func_800B9798(0, 0);
    ovl_21_func_800B9798(1, 0);
    ovl_21_func_800BA4C0();
    ready = func_80013394();
    if (ready == 1) {
        if ((s16) D_800C0448_init.records[0].unk2 != 0) {
            func_8002261C(4, 0x29);
            state = GetVal8005E5B4();
            if (state == 2) {
                if (GetVal8005E5B8() == ready) {
                    D_800C0448[0] = state;
                } else {
                    D_800C0448[0] = ready;
                }
            }
        } else {
            D_800C0448[0] = ready;
        }
    }
}
