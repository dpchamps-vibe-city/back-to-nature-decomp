#include "common.h"

extern s16 gProgressRequest;

s32 ovl_11_func_800FDFD8(s16 arg0) {
    return gProgressRequest == arg0;
}
