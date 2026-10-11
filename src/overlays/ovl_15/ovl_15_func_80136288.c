#include "common.h"
#include "game_types.h"
#include "psyq/libmcrd.h"

void ovl_15_func_80137544(char *arg0, s32 arg1);

s32 ovl_15_func_80136288(s16 arg0, s16 arg1, Ovl15SaveSummary *arg2) {
    union {
        unsigned long words[32];
        Ovl15SaveSummary summary;
        u8 bytes[0x80];
    } buffer;
    long command;
    long result;
    s32 channel;
    s32 i;
    s32 status;
    u8 checksum;

    channel = 0;
    if (arg0 != 0) {
        status = 0xC;
        if (arg0 == 1) {
            channel = 0x10;
        } else {
            return status;
        }
    }
    ovl_15_func_80137544(Ovl15CardFilename, arg1);
    Ovl15CardRetries = 0;
    MemCardSync(0, &command, &result);
retry:
    MemCardReadFile(channel, Ovl15CardFilename, buffer.words, 0x200, 0x80);
    MemCardSync(0, &command, &result);
    switch (result) {
    case 1:
        if ((s16) Ovl15CardRetries >= 7) {
            return 4;
        }
        Ovl15CardRetries++;
        goto retry;
    case 2:
        if ((s16) Ovl15CardRetries >= 7) {
            return 8;
        }
        Ovl15CardRetries++;
        goto retry;
    case 5:
        if ((s16) Ovl15CardRetries < 7) {
            Ovl15CardRetries++;
            goto retry;
        }
        return 9;
    case 3:
        return 5;
    case 0:
    case 4:
    default:
        *arg2 = buffer.summary;
        checksum = 0;
        for (i = 0; i < 0x7F; i++) {
            checksum ^= buffer.bytes[i];
        }
        if (checksum != buffer.bytes[0x7F]) {
            return 0xE;
        }
        return 0;
    }
}
