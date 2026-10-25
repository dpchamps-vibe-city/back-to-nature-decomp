#include "common.h"
#include "cd_loader.h"

extern s32 D_8005E3B0;
extern s32 D_8009A3F8;

void func_8001719C(u8 *arg0);

void ovl_11_func_800DD21C(s32 *arg0, s32 *arg1, u32 arg2);

void ovl_11_func_800BD538(void) {
    CdReadDiscSectorsBlocking(0, 0x0380B000, 0x5000, 0, D_8005E3B0 + 0x4290);
    ovl_11_func_800DD21C(&D_8009A3F8, D_8005E3B0 + 0x4290, 0x540U);
    func_8001719C(D_8005E3B0 + 0x5790);
}
