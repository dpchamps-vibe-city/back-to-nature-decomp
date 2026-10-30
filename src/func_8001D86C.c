/* Look up a 16-bit code in the parallel code/result tables; return 0 if absent. */

#include "common.h"

s32 func_8001D86C(u16 code) {
    s32 i;

    for (i = 0; i < 20; i++) {
        if (D_800491F0[i] == code) {
            return D_80049218[i];
        }
    }
    return 0;
}
