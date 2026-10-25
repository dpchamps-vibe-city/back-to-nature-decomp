#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

#include "color_row_blend.h"

s16 ovl_11_func_80111D28(s16 arg0);
s32 ovl_11_func_80111CC4(s16 arg0, s16 arg1);

#define BUTTON ((struct struct_8006C838_button *)D_8006C838)

void ovl_11_func_80111A60(void) {
    s16 temp_v0;
    s16 var_a2;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_v0;

    gOvl11ColorRowProgress = 0;
    temp_v0 = ovl_11_func_80111D28(BUTTON->field_44BA);
    temp_v0_2 = ovl_11_func_80111CC4(BUTTON->field_44C0, temp_v0);
    if (temp_v0_2 != 2) {
        if (temp_v0_2 < 3) {
            var_a2 = 0x71;
            if (temp_v0_2 < 0) {
                goto Lneg;
            }
            var_v0 = 0xDF;
            goto Ldone;
        }
        if (temp_v0_2 == 3) {
            goto Lr3;
        }
        var_v0 = 0;
        var_a2 = 0;
        goto Ldone;
    }
    temp_v1 = ((BUTTON->field_44C0 - temp_v0) * 0x3C) + BUTTON->field_44C2;
    gOvl11ColorRowProgress = temp_v1;
    if (temp_v1 >= 0x71) {
        gOvl11ColorRowProgress = 0x71;
    }
    var_a2 = 0x71;
    var_v0 = 0xDF;
    goto Ldone;
Lr3:
    var_a2 = 0x20;
    var_v0 = 0xDE;
    goto Ldone;
Lneg:
    var_v0 = 0;
    var_a2 = 0;
Ldone:
    InitColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowSources, var_a2, 0x140, var_v0, 0x140, var_v0 - 1, 0x140, 0xDC);
}
