#include "common.h"
#include "screen_primitives.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

void ovl_11_func_80103964(void);

void ovl_11_func_801038E4(void) {
    if (D_80127428 != 0) {
        SetTextSpacing(0, 2);
        DrawScreenTileRect((u32 *)(gActiveRenderContext->field_D8 + 0x60), 0, 0, (s16)HWD0, (s16)VWD0, 0, 1);
        ovl_11_func_80103964();
        SetTextSpacing(0, 0);
    }
}
