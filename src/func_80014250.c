#include "common.h"

s32 func_800142D8(s32, s32);

s32 func_80014250(s32 arg0) {
    if ((gPadReadBuffers[arg0][1] >> 4) != 4) {
        if ((gPadReadBuffers[arg0][1] >> 4) == 7) {
            if ((gPadReadBuffers[arg0][2] & 0xF0) == 0xF0) {
                return func_800142D8(gPadReadBuffers[arg0][6], gPadReadBuffers[arg0][7]);
            }
        }
        return -1;
    }
    return -1;
}
