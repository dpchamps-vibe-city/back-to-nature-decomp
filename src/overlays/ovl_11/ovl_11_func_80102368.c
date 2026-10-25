#include "common.h"

s32 pow_int(s32 arg0, s32 arg1);
void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800F6D14(s32 *arg0, u16 arg1, s16 arg2, s16 arg3);

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
            ovl_11_func_800D6F78(id, 0, context, x, y, 8);
        } else {
            ovl_11_func_800F6D14(context, id, x, y);
        }
        x += 0x22;
        if (x >= 0xEBU) {
            x = 0x84;
            y += 0x22;
        }
    }
}
