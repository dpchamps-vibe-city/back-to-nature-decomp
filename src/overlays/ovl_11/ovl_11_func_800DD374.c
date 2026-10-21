#include "common.h"
#include "game_types.h"

void ovl_11_func_800DD374(void) {
    s16 var_a1;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    u16 temp_a0;
    u8 *var_a2;

    var_a2 = D_80128E08;
    D_80129178[0] = 0;
    D_80129178[1] = 0;
    D_80129178[2] = 0;
    D_80129184 = 0;
    for (var_a3 = 0; var_a3 < 15; var_a3++, var_a2 += 0x30) {
        temp_a0 = *(u16 *)(var_a2 + 0x28);
        if (temp_a0 & 0x100) {
            temp_v1 = temp_a0 & 0xF;
            switch (temp_v1) {
            case 0:
                *(u16 *)(var_a2 + 0x28) = temp_a0 & 0xEFFF;
                var_a1 = 8;
                var_a0 = 0;
                break;
            case 1:
                *(u16 *)(var_a2 + 0x28) = temp_a0 | 0x1000;
                var_a1 = 4;
                var_a0 = 1;
                break;
            case 2:
                *(u16 *)(var_a2 + 0x28) = temp_a0 | 0x1000;
                var_a1 = 12;
                var_a0 = 2;
                break;
            default:
                var_a1 = 0;
                var_a0 = 0;
                break;
            }
            *(s16 *)(*(u8 **)(var_a2 + 0x2C) + 0x2E) = 0;
            *(s16 *)(*(u8 **)(var_a2 + 0x2C) + 0x30) = var_a1;
            D_80129178[var_a0] = var_a2;
        }
    }
}
