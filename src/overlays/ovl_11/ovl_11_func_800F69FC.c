#include "common.h"

/* Start a new activity pass; IDs and texture geometry remain cached. */
void ovl_11_func_800F69FC(void) {
    s32 i;

    i = 0x47;
    do {
        gOvl11TextureCache[i].active = 0;
        i--;
    } while (i >= 0);
}
