#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800F44B4(u16 arg0, s32 *arg1, s16 *arg2, s32 arg3);
s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);
void ovl_11_func_800F4CEC(void);
void ovl_11_func_800F4CAC(void);
void ovl_11_func_800F4FC8(void);

void ovl_11_func_800F4D28(void) {
    s32 sp20[4];
    Ovl11CollisionTag sp30;
    s32 var_s1;
    s32 var_s2;

    var_s1 = 0;
    if ((ovl_11_func_800F44B4(0x64, sp20, &sp30.field_2, 0) == 0) &&
        (((Ovl11SpawnProbeState *) D_8006C838)->flags & 0x800) &&
        (((Ovl11SpawnProbeState *) D_8006C838)->selected == 0x64)) {
        var_s1 = 1;
        ((Ovl11SpawnProbeState *) D_8006C838)->record[2] |= 1;
    }
    if (ovl_11_func_800BE208(sp20, sp30, 0, 0, NULL, NULL, 0, 0x20) != 0) {
        var_s1 = 1;
    }
    var_s2 = 0;
    if (var_s1 != 0) {
        ovl_11_func_800F4CEC();
    }
    ovl_11_func_800F44B4(0x65, sp20, &sp30.field_2, 0);
    var_s1 = ovl_11_func_800BE208(sp20, sp30, 0, 0, NULL, NULL, 0, 0x20) != 0;
    if (sp30.field_2 >= 7) {
        goto invalid;
    }
    if (sp30.field_2 > 0) {
        goto valid;
    }
invalid:
    var_s2 = 1;
valid:
    if (var_s1 != 0) {
        ovl_11_func_800F4CAC();
    }
    if (var_s2 != 0) {
        ovl_11_func_800F4FC8();
    }
}
