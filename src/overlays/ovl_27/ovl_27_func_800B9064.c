#include "common.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B92E4(void);
s32 ovl_27_func_800B90DC(void);

extern s32 D_800C4A14;
extern s16 D_800C4A3C;

void ovl_27_func_800B9064(void) {
    ovl_27_func_800B92E4();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawBackdropAnim33(D_800C4A3C);
    D_800C4A3C = D_800C4A3C - 2;
    if (D_800C4A3C <= -HWD0) {
        D_800C4A14 = (s32)ovl_27_func_800B90DC;
    }
}
