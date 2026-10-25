#include "common.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B92E4(void);
void ovl_27_func_800B9064(void);

extern s32 D_800C4A14;
extern s32 D_800C4A1C;

void ovl_27_func_800B8FC8(void) {
    ovl_27_func_800B92E4();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    if ((D_800C4A1C % 30) < 0xF) {
        Ovl27DrawBackdropAnim33(0);
    }
    if (D_800C4A1C >= 0x5A) {
        D_800C4A14 = (s32)ovl_27_func_800B9064;
    }
}
