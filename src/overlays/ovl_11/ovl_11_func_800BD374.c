#include "common.h"
#include "cd_loader.h"

extern s32 D_801217A8;

s32 ovl_11_func_800BD374(s32 arg0, s32 arg1) {
    u8 *temp_v1;
    s32 temp_a1;

    temp_v1 = (arg0 * 0x10) + (u8 *)&D_801217A8;
    temp_a1 = *(s32 *)temp_v1;
    return CdReadArchiveBytes(0, temp_a1, *(s32 *)(temp_v1 + 4) - temp_a1,
                         (u8 *)D_8007AFF0, 1, arg1) != 0;
}
