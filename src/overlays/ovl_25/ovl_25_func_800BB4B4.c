#include "common.h"
#include "psyq/memory.h"

void ovl_25_func_800BB4B4(void) {
    char *far_base;
    u32 i;

    memset(&gVramAnimationStoragePrefix, 0, 0xB4);
    far_base = (char *)&D_8007AFF0;
    for (i = 0; i < 8; i++) {
        *(u16 *)(far_base + 0x253C0 + i * 0x16) = 0xFFFF;
    }
}
