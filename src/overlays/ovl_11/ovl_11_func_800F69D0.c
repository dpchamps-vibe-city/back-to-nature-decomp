#include "common.h"

/* Clear occupancy and activity without erasing texture geometry. */
void ovl_11_func_800F69D0(void) {
    s32 i;

    for (i = 0; i <= 0x47; i++) {
        gOvl11TextureCache[i].active = 0;
        gOvl11TextureCache[i].id = 0;
    }
}
