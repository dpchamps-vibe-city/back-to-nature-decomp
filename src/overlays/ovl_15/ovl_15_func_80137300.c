#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

/* D_80137828 is an ovl_15 data word not yet classified by the generated
 * globals; the target loads it absolutely (lui + lw) and passes the loaded
 * pointer to func_80014CBC. */
extern u8 *D_80137828;

s32 ovl_15_func_80137300(void) {
    s32 temp_v0;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x10, 0, 0x900, D_80137828, 1, 1);
    do {
        temp_v0 = CdReadArchiveBytes(0x10, 0, 0x900, D_80137828, 1, 0);
    } while (temp_v0 == 0);
    return temp_v0;
}
