#include "common.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B92E4(void);
void ovl_27_func_800B8F58(void);

extern s32 D_800C4A14;
extern s16 D_800C4A3C;

s32 ovl_27_func_800B90DC(void) {
    ovl_27_func_800B92E4();
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    D_800C4A3C = 0x140;
    D_800C4A14 = (s32)ovl_27_func_800B8F58;
    return (s32)ovl_27_func_800B8F58;
}
