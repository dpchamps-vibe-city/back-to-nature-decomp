#include "common.h"
#include "psyq/libsnd.h"

s32 func_80020540(s32 arg0) {
    s32 result;

    result = SsSetReservedVoice(arg0 & 0xFF);
    if ((result << 24) < 0) {
        return -1;
    }
    return 0;
}
