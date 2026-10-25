#include "common.h"
#include "screen_fade.h"
#include "sprite_grid.h"

void ovl_11_func_800FBE88(void);
void ovl_11_func_800FC1AC(void);
void ovl_11_func_800BCF28(void);

void ovl_11_func_800FC054(void) {
    s32 temp_s0;

    temp_s0 = IsScreenFadeComplete();
    if (D_8012720C >= 3 && D_8012720C <= 5) {
        DrawScrollingSpriteGridInContext(gActiveRenderContext->field_D8 + 0x74);
    }
    switch (D_8012720C) {
    case 1:
        if (temp_s0 == 1) {
            D_8012720C = 2;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x20000;
            return;
        }
    default:
        return;
    case 2:
        ovl_11_func_800FBE88();
        BeginScreenFadeDecrease(0xA, 0, 2);
        D_8012720C = 3;
        return;
    case 3:
        if (temp_s0 == 1) {
            D_8012720C = 4;
            return;
        }
        break;
    case 4:
        ovl_11_func_800FC1AC();
        return;
    case 5:
        if (temp_s0 == 1) {
            BeginScreenFadeDecrease(0xA, 0, 2);
            D_8012720C = 6;
            ((struct struct_8006C838_view *) D_8006C838)->field_0C |= 0x40000;
        }
        break;
    case 6:
        if (temp_s0 == 1) {
            D_8012720C = 0;
            ovl_11_func_800BCF28();
        }
        break;
    }
}
