#include "common.h"
#include "cd_loader.h"

s32 ovl_11_func_800F71DC(s32 arg0) {
    return CdReadArchiveBytes(0, (arg0 << 0x10 >> 5) + 0x3C19000, 0x28000, D_8005E3B0 + 0x4290, 1, 1);
}
