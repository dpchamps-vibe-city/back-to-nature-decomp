#include "common.h"
#include "psyq/memory.h"

/* Tentative definitions for GP-relative globals owned by this TU. */
s16 D_8005E3E8[2];
s32 D_8005E3EC;

void func_80014494(s32 arg0, s32 arg1, s32 arg2) {
    u8 act0;
    u8 act1;

    act0 = arg0 & 0xFF;
    act1 = arg1 & 0xFF;

    if (D_8005E3E8[arg2] == 0) {
        gPadActuatorBuffers[arg2][0] = 0x40;
        gPadActuatorBuffers[arg2][1] = (act0 > 0);
        if (act1 != 0) {
            gPadActuatorBuffers[arg2][1] = 1;
        }
        gPadActuatorBuffers[arg2][2] = 0;
    } else {
        gPadActuatorBuffers[arg2][1] = (act0 > 0);
        gPadActuatorBuffers[arg2][2] = act1;
    }

    if (D_8005E3EC == 0) {
        memset(gPadActuatorBuffers, 0, 0x10);
    }
}
