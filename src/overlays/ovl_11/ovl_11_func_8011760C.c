#include "common.h"

s32 ovl_11_func_8011760C(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = gChoiceCellStates;
    while (i < 9) {
        if (*p == 2) {
            return i;
        }
        i++;
        p++;
    }
    return -1;
}
