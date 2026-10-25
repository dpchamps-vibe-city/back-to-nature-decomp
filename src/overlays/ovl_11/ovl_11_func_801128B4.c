#include "common.h"
#include "psyq/memory.h"
#include "game_flags.h"

void ovl_11_func_801128B4(void) {
    char *base;
    char *base2;

    memset((u8 *)D_80074838 + 0x676C, 0x34, 0);
    base = (char *)&D_8006C838;
    base2 = base + 0x8000;
    *(s16 *)(base2 + 0x6776) = -1;
    SetGameFlag(0x1B, 1);
}
