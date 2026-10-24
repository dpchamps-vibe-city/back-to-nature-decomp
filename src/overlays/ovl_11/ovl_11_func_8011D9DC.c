#include "common.h"

s32 pow_int(s32, s32);
/* The original caller supplies two additional, unused zero arguments. */
s32 ovl_11_func_800E6EAC();

s16 ovl_11_func_8011D9DC(s16 id, s32 mode) {
    s32 count;
    s32 total;
    s32 i;
    s16 *slot;

    if (mode == 0) {
        if (ovl_11_func_800E6EAC(0, 2, 0, 0) == 1) {
            return 99;
        }
    }
    total = 0;
    count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
    slot = (s16 *)&((struct Ovl11ItemInsertState *)D_8006C838)->held;
    if (slot[0] == 0) {
        total = 99;
    } else if (slot[0] == id) {
        total = 99 - slot[2];
    }
    slot = (s16 *)&D_80071A90;
    for (i = 0; i < count; i++, slot += 3) {
        if (slot[0] == 0) {
            total += 99;
        } else if (slot[0] == id) {
            total += 99 - slot[2];
        }
    }
    return (s16)total;
}
