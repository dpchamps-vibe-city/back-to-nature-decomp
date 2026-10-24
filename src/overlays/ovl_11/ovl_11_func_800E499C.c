#include "common.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"

s32 GetVal8005E3B0(void);
s32 ovl_11_func_800E4AEC(s32 arg0);
void ovl_11_func_800E5078(void);
s32 ovl_11_func_800E4C30(s32 arg0);
s32 ovl_11_func_800E4C84(s32 arg0);
s32 ovl_11_func_800E4B6C(s32 arg0, s32 arg1);
void ovl_11_func_800E4BA4(s32 arg0);
void ovl_11_func_800E4D08(s32 arg0);
void ovl_11_func_800E4DAC(s32 arg0);
void ovl_11_func_800CF36C(s32 arg0);

void ovl_11_func_800E499C(s32 arg0) {
    s32 size;
    u8 *destination;
    u32 recordSize;
    s32 index;

    D_80129410 = 0x180;
    D_80129412 = 0;
    if (*(u16 *)arg0 & 0x20) {
        D_80129410 = 0;
        D_80129412 = 0;
        ((struct Ovl11PoolResetView *)D_8006C838)->flags |= 0x10;
    }
    size = ovl_11_func_800E4AEC(arg0);
    destination = (u8 *)(GetVal8005E3B0() - size);
    D_80129414 = size;
    ovl_11_func_800E5078();
    recordSize = ovl_11_func_800E4C30(arg0);
    if (recordSize > 0xCE0) {
        SystemError(0x53, Ovl11CurrentObjectId[1]);
    }
    index = ovl_11_func_800E4C84(arg0);
    memcpy(D_8006E910, (void *)ovl_11_func_800E4B6C(arg0, index), recordSize);
    ovl_11_func_800E4BA4(arg0);
    if (size != 0) {
        memmove(destination, (u8 *)ovl_11_func_800E4B6C(arg0, 0), size);
        D_801291B8 = destination;
        ovl_11_func_800E4D08(arg0);
        ovl_11_func_800E4DAC(arg0);
        ovl_11_func_800CF36C(1);
    } else {
        D_801291B8 = 0;
    }
}
