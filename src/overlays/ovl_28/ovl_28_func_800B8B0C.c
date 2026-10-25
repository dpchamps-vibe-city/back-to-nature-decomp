#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

void ovl_28_func_800B8B0C(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x3D77000, 0x6000, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
}
