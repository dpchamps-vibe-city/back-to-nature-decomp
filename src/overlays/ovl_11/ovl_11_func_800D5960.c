#include "common.h"

typedef struct {
    /* 0x00 */ char pad_00[0x20];
    /* 0x20 */ void *field_20;
    /* 0x24 */ void *field_24;
} D8006C838Lookup_800D5960;

void ovl_11_func_800D6488(u8 arg0, u8 dst[][13], u8 src[][13]);

s32 ovl_11_func_800D5960(s16 *arg0, u32 arg1, s16 arg2, s16 *arg3, s16 *arg4, s16 arg5) {
    D8006C838Lookup_800D5960 *v = (D8006C838Lookup_800D5960 *)&D_8006C838;
    u8 sp10[13][13];
    s16 temp_v0;
    s32 var_a1;
    s32 var_a2;
    s32 var_t1;

    temp_v0 = *(s16 *)((char *)v->field_20 + (*arg0 * 0x28) + (arg5 * 0x28) + 0x0E);
    if (temp_v0 == -1) {
        return 0;
    }
    ovl_11_func_800D6488(arg2 & 0xFF, sp10, (u8 (*)[13])((char *)v->field_24 + temp_v0 * 0xB0));
    arg1 = arg1 + 1;
    var_t1 = 0;
    for (var_a2 = 0; var_a2 < 13; var_a2++) {
        for (var_a1 = 0; var_a1 < 13; var_a1++) {
            if (sp10[var_a2][var_a1] == arg1) {
                var_t1 = 1;
                *arg3 = var_a1 - 6;
                *arg4 = var_a2 - 6;
                break;
            }
        }
    }
    return var_t1;
}
