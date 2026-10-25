#include "common.h"
#include "screen_primitives.h"

void func_80017A38(s16 arg0, s16 arg1);
void ovl_11_func_80103964(void);

void ovl_11_func_801038E4(void) {
    if (D_80127428 != 0) {
        func_80017A38(0, 2);
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x60), 0, 0, (s16)HWD0, (s16)VWD0, 0, 1);
        ovl_11_func_80103964();
        func_80017A38(0, 0);
    }
}
