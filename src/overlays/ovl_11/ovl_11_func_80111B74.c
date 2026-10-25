#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

#include "color_row_blend.h"

s16 ovl_11_func_80111D28(s16 arg0);
s32 ovl_11_func_80111CC4(s16 arg0, s16 arg1);

#define BUTTON ((struct struct_8006C838_button *)D_8006C838)

void ovl_11_func_80111B74(void) {
    s16 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    temp_v0 = ovl_11_func_80111D28(BUTTON->field_44BA);
    temp_v0_2 = ovl_11_func_80111CC4(BUTTON->field_44C0, temp_v0);
    if (temp_v0_2 != 2) {
        if (temp_v0_2 >= 3) {
            if (temp_v0_2 == 3) {
                goto Lr3;
            }
        } else {
            if (temp_v0_2 < 0) {
                return;
            }
            DrawColorRowBlend(&gOvl11ColorRowBlend, 0);
        }
        return;
    }
    if (gOvl11ColorRowProgress < 0x71) {
        DrawColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowProgress);
        temp_v0_3 = (BUTTON->field_44C0 - temp_v0) * 0x3C + BUTTON->field_44C2;
        gOvl11ColorRowProgress = temp_v0_3;
        if (temp_v0_3 >= 0x71) {
            gOvl11ColorRowProgress = 0x71;
            return;
        }
    } else {
        if (gOvl11ColorRowProgress == 0x71) {
            InitColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowSources, 0x20, 0x140, 0xDE, 0x140, 0xDD, 0x140, 0xDC);
            gOvl11ColorRowProgress += 1;
            return;
        }
        return;
    }
    return;
Lr3:
    StepColorRowBlend(&gOvl11ColorRowBlend, 1, 1);
}
