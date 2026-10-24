#include "common.h"
#include "game_types.h"

/* Overlay alias of the matched signed state-word getter GetVal8005E5B4. */
s32 GetVal8005E5B4(void);
void func_80022738(void);
s32 ovl_11_func_800F2724(void);
void ovl_11_func_800F3840(void);

void ovl_11_func_800F2400(void) {
    if (D_80126C90 == 0) {
        D_80126C94 = 0;
    } else {
        if (GetVal8005E5B4() == 2) {
            if (D_80126C94 != 0) {
                D_80126C90 = 0;
                D_80126C94 = 0;
                ovl_11_func_800F3840();
            } else {
                if (ovl_11_func_800F2724() == 0) {
                    D_80126C90 = 0;
                }
            }
        } else if (((GfxObj *)D_8005E3A8)->field_8 & 0x20) {
            D_80126C90 = 0;
            func_80022738();
        }
    }
}
