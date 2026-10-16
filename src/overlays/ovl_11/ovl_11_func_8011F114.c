#include "common.h"

s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
s32 ovl_11_func_8011F1D0(s32 arg0);

void ovl_11_func_8011F114(void) {
    if (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 0x15) {
        switch ((u32) D_8012DB10) {
        case 0:
            return;
        case 1:
            func_8002261C(2, ovl_11_func_8011F1D0(D_8012DB14));
            D_8012DB10 = 2;
            break;
        case 2:
            if (func_800226A4() == 2) {
                D_8012DB10 = 3;
            }
            break;
        case 3:
            D_8012DB10 = 0;
            break;
        }
    }
}
