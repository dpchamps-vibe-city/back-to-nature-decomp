#include "common.h"
#include "psyq/libmcrd.h"

s32 ovl_15_func_80136470(s16 arg0) {
    long command;
    long result;
    s32 channel;
    s32 status;

    channel = 0;
    if (arg0 != 0) {
        status = 0xC;
        if (arg0 == 1) {
            channel = 0x10;
        } else {
            return status;
        }
    }
    Ovl15CardRetries = 0;
    MemCardSync(0, &command, &result);
retry:
    result = MemCardFormat(channel);
    switch (result) {
    case 1:
        if ((s16) Ovl15CardRetries >= 7) {
            return 4;
        }
        Ovl15CardRetries++;
        goto retry;
    case 2:
        if ((s16) Ovl15CardRetries < 7) {
            Ovl15CardRetries++;
            goto retry;
        }
        return 2;
    case 0:
        return 0;
    default:
        return 2;
    }
}
