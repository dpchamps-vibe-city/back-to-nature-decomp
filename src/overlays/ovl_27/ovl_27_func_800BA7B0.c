#include "common.h"
#include "cd_loader.h"

/* Callee prototypes as the original caller TU saw them. */
void func_8001719C(u8 *arg0);

s32 ovl_27_func_800BA7B0(void) {
    if (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0) {
        return 0;
    }
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x4290));
    return 1;
}
