#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void ovl_11_func_800C47F0(void) {
    s32 temp_a1;
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if (((struct struct_8006C838_800C47F0 *) D_8006C838)->field_E4CC == 3) {
        if ((*(s16 *) ((u8 *) gPad0Input + 0x34)) >= 0x56) {
            D_801231F4 |= 0x20;
        }
    }
    if (D_80128CF8 != 0) {
        var_a2 = 1;
    }
    if (((struct struct_8006C838_800C47F0 *) D_8006C838)->field_E4CC == 1) {
        var_a2 = 1;
    }
    if (((struct struct_8006C838_800C47F0 *) D_8006C838)->field_E4CC == 0) {
        var_a2 = 0;
    }
    if (((struct struct_8006C838_800C47F0 *) D_8006C838)->field_E4CC == 3) {
        var_a2 = 0;
    }
    if (var_a2 != 0) {
        temp_a1 = D_801231F4;
        temp_t0 = D_801231F8;
        temp_v1 = temp_a1 & 0xFFFF0FFF;
        temp_v0 = temp_a1 & 0x1000;
        D_801231F4 = temp_v1;
        D_801231F8 = temp_t0 & 0xFFFF0FFF;
        if (temp_v0 != 0) {
            D_801231F4 = temp_v1 | 0x8000;
        }
        if (temp_t0 & 0x1000) {
            D_801231F8 = (temp_t0 & 0xFFFF0FFF) | 0x8000;
        }
        if (temp_a1 & 0x4000) {
            D_801231F4 |= 0x2000;
        }
        if (temp_t0 & 0x4000) {
            D_801231F8 |= 0x2000;
        }
        if (temp_a1 & 0x8000) {
            D_801231F4 |= 0x4000;
        }
        if (temp_t0 & 0x8000) {
            D_801231F8 |= 0x4000;
        }
        if (temp_a1 & 0x2000) {
            D_801231F4 |= 0x1000;
        }
        if (temp_t0 & 0x2000) {
            D_801231F8 |= 0x1000;
        }
    }
}
