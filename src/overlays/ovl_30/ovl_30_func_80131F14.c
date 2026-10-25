#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_80131F14(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    u32 var_s0;
    struct_80076220 *rec;

    if (*(s32 *)gPad0Input & 4) {
        temp_v0 = D_801349B0 - 5;
        D_801349B0 = temp_v0;
        if (temp_v0 <= 0) {
            D_801349B0 = 0x24;
        }
    }
    if (*(s32 *)gPad0Input & 8) {
        temp_v0_2 = D_801349B0 + 5;
        D_801349B0 = temp_v0_2;
        if (temp_v0_2 >= 0x25) {
            D_801349B0 = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        temp_v0_3 = D_801349B0 - 1;
        D_801349B0 = temp_v0_3;
        if (temp_v0_3 <= 0) {
            D_801349B0 = 0x24;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        temp_v0_4 = D_801349B0 + 1;
        D_801349B0 = temp_v0_4;
        if (temp_v0_4 >= 0x25) {
            D_801349B0 = 0;
            var_s0 = 0;
        }
    }
    var_s0 = 0;
    do {
        rec = &gObjectEventSlotStorage[(D_801349B0 + var_s0) % 0x25];
        FntPrint(&D_8012ECD8, (D_801349B0 + var_s0) % 0x25, *(s32 *)rec->unk28);
        var_s0 += 1;
    } while (var_s0 < 0xAU);
    return 0;
}
