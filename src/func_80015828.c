#include "common.h"

/* Flag operations pass a word mask; normalize it before modifying u16 flags. */
void func_80015828(u16 *flags, s32 mask) {
    mask &= 0xFFFF;
    *flags &= ~mask;
}
