#include "common.h"
#include "integer_power.h"
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

void ovl_11_func_80102368(void) {
    u16 x;
    u16 y;
    u16 id;
    s32 i;
    s32 j;
    s32 *context;

    x = 0x84;
    y = 0x14;
    context = (s32 *)(gActiveRenderContext->field_D8 + 0x68);
    for (i = 0; i < 8; i++) {
        id = D_8012CF00_ids[i];
        for (j = 0; j < pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1); j++) {
            if (((struct Ovl11ItemGridState *)D_8006C838)->slots[j].id == id) {
                break;
            }
        }
        if (j != pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1) && id != 0) {
            Ovl11DrawSlotSprite(id, 0, context, x, y, 8);
        } else {
            Ovl11DrawTextureCell(context, id, x, y);
        }
        x += 0x22;
        if (x >= 0xEBU) {
            x = 0x84;
            y += 0x22;
        }
    }
}
