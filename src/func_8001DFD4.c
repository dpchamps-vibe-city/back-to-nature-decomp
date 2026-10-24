#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"

s32 func_8001DFD4(s32 *arg0, SVECTOR *arg1) {
    long flag;
    SVECTOR sxy;

    gte_ldv0(arg1);
    gte_rtps();
    gte_stflg(&flag);
    if (flag < 0) {
        return 0;
    }
    gte_stsxy2(&sxy);
    if (arg0 != 0) {
        arg0[0] = sxy.vx;
        arg0[1] = sxy.vy;
    }
    gte_stszotz(&flag);
    return flag;
}
