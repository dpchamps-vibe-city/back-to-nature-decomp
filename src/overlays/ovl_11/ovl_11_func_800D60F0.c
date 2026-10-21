#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800D60F0(s16 arg0, s16 arg1) {
    s16 row;
    s32 var_v1;
    u16 temp_a3;
    ItemData *temp_v0;
    Ovl11PackedItemGlobals *globals = (Ovl11PackedItemGlobals *)D_8006C838;
    s32 *properties;
    s32 *address;
    s32 shift;

    temp_v0 = (ItemData *)(arg0 * 0x28 + (s32)globals->items);
    row = temp_v0->field_0E;
    temp_a3 = temp_v0->u0.flags;
    if (row == -1) {
        return -1;
    }
    if (temp_a3 & 0x4000) {
        return -2;
    }
    properties = (s32 *)(row * 0x10 + (s32)globals->properties);
    address = &properties[(s16)(arg1 / 10)];
    shift = (s16)(arg1 % 10) * 3;
    var_v1 = *address >> shift;
    var_v1 &= 7;
    if ((temp_a3 & 0x180) && (var_v1 >= 0)) {
        if (var_v1 < 3) {
            var_v1 = 5;
        } else if (var_v1 < 5) {
            var_v1 = 6;
        }
    }
    return var_v1;
}
