#include "common.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B92E4(void);
void ovl_27_func_800B8FC8(void);

extern s32 D_800C4A14;
extern s32 D_800C4A1C;
extern s16 D_800C4A3C;

void ovl_27_func_800B8F58(void) {
    ovl_27_func_800B92E4();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawBackdropAnim33(D_800C4A3C);
    D_800C4A3C = D_800C4A3C - 2;
    if (D_800C4A3C <= 0) {
        D_800C4A1C = 0;
        D_800C4A14 = (s32)ovl_27_func_800B8FC8;
    }
}
