#include "common.h"
#include "cd_loader.h"

s32 ovl_27_func_800BA578(void) {
    return CdReadArchiveBytes(0, 0x2A800, 0x49000, (u8 *)D_8007AFF4, 1, 0) != 0;
}
