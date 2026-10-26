#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"

void ovl_15_func_80137544(char *arg0, s32 arg1);

s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1) {
    struct DIRENTRY entries[15];
    long sync[2];
    long found;
    long files;
    long blocks;
    s32 rounded;
    s32 i;
    s32 required;
    s32 channel;
    s32 status;

    required = 4;
    found = 0;
    files = 0;
    if (arg1 == 3) {
        required = 10;
    }
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
    gOvl15CardRetryCount = 0;
    memset(entries, 0, sizeof(entries));
    MemCardSync(0, &sync[0], &sync[1]);
retry_first:
    sync[1] = MemCardGetDirentry(channel, Ovl15CardFilename, entries, &found, 0, 1);
    switch (sync[1]) {
    case 1:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_first;
        }
        return 4;
    case 2:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_first;
        }
        return 8;
    case 0:
    default:
        gOvl15CardRetryCount = 0;
        break;
    }
    memset(entries, 0, sizeof(entries));
    MemCardSync(0, &sync[0], &sync[1]);
retry_second:
    sync[1] = MemCardGetDirentry(channel, Ovl15CardWildcard, entries, &files, 0, 15);
    switch (sync[1]) {
    case 1:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_second;
        }
        return 4;
    case 2:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_second;
        }
        return 8;
    case 0:
    default:
        break;
    }
    blocks = 0;
    for (i = 0; i < 15; i++) {
        long quotient = entries[i].size / 8192;
        if (entries[i].size % 8192) {
            rounded = blocks + 1;
            blocks = rounded + quotient;
        } else {
            blocks += quotient;
        }
    }
    status = 0;
    if (found == 0) {
        if (blocks != 15) {
            if ((15 - blocks) < required) {
                return 0xB;
            }
            return 0;
        }
        return 15;
    }
    return status;
}
