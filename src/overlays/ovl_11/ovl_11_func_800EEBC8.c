#include "common.h"
#include "psyq/memory.h"

/* Original calls at 800E407C/800E43F0 explicitly supply four zero words
 * in a0..a3. The reset ignores them; retain that witnessed calling contract. */
s32 ovl_11_func_800EEBC8(s32 unused0, s32 unused1, s32 unused2, s32 unused3) {
    memset(gOvl11ValueSlots, 0, 0x50);
    return 1;
}
