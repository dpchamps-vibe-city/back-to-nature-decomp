#include "common.h"
#include "psyq/libsnd.h"

s32 func_80020790(s16 arg0) {
    s32 temp;

    SsUtSetVVol(arg0, 0, 0);
    temp = SsUtKeyOffV(arg0);
    if (temp < 0) {
        return -1;
    }
    return 0;
}
