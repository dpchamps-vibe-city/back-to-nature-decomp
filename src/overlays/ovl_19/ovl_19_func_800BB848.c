#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);

void ovl_19_func_800BB848(void) {
    DrawSync(0);
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x4290));
}
