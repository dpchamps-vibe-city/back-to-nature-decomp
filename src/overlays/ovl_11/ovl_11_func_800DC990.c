#include "common.h"

void ovl_11_func_800DCA10(void *);

void ovl_11_func_800DC990(void) {
    u8 *base;
    s32 i;

    base = gOvl11SceneRecords;
    for (i = 0xE; i >= 0; i--) {
        ovl_11_func_800DCA10(base);
        base += 0x30;
    }
}
