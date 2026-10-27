#include "common.h"
#include "psyq/memory.h"
#include "ovl11_texture_cell_cache.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "psyq/abs.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_slot_decorations.h"

s32 ovl_11_func_800F8404(s32 arg0);

void ovl_11_func_800F889C(s32 *arg0, s32 arg1, s16 *arg2) {
    if (arg2[0] == 0) {
        Ovl11DrawEmptySlot((s32) (arg0 + 1), D_80126E54[arg1].x, D_80126E54[arg1].y);
        return;
    }
    if (arg1 < 9) {
        Ovl11DrawSlotSprite(arg2[0], 0, arg0, D_80126E54[arg1].x, D_80126E54[arg1].y, 8);
    } else {
        Ovl11DrawTextureCell(arg0, (u16) arg2[0], D_80126E54[arg1].x, D_80126E54[arg1].y);
    }
    switch (Ovl11GetItemType(arg2[0])) {
    case 2:
        Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, D_80126E54[arg1].x, D_80126E54[arg1].y, arg2[2]);
        return;
    case 0:
        if (ovl_11_func_800F8404(arg1) == 2) {
            Ovl11DrawSlotQuantity(gActiveRenderContext->field_D8 + 0x58, D_80126E54[arg1].x, D_80126E54[arg1].y, arg2[2]);
        }
        /* fallthrough */
    case 1:
        return;
    }
}
