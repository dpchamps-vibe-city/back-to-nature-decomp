#include "common.h"
#include "game_flags.h"

void ovl_11_func_801129A0(void) {
    char *base;
    u16 v1;
    u16 a0v;

    base = (char *)&D_8006C838;
    v1 = *(u16 *)(base + 0x44BA);
    a0v = *(u16 *)(base + 0x44BC);
    base += 0x8000;
    *(u16 *)(base + 0x5BDC) = v1;
    *(u16 *)(base + 0x5BDE) = a0v;
    SetGameFlag(0x4E, 0);
    *(u16 *)(base + 0x6770) = 0;
}
