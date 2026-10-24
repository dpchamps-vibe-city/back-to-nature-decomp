#include "common.h"
#include "game_types.h"

/* Return the first occupied slot in the selected direction, or the original
 * index if all nine probes miss. The mode follows the by-value record. */
s16 ovl_11_func_8011D890(Ovl11D548Arg x, s32 mode) {
    s16 j;
    s32 i;

    i = 1;
    for (; i < 10; i++) {
        if (mode == 1) {
            j = x.index + i;
        } else {
            j = x.index - i;
        }
        j = (j >= 10) ? j - 10 : j;
        j = (j < 0) ? j + 10 : j;
        if (*(s32 *)((char *)&x + ((s32)j << 2)) != 0) {
            return j;
        }
    }
    return x.index;
}
