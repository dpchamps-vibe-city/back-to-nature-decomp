#include "common.h"

s32 ovl_15_func_80133808(s16 arg0);
void ovl_15_func_80134724(void);

void ovl_15_func_8013203C(void) {
    s32 temp_v0;

    temp_v0 = ovl_15_func_80133808(1);
    switch (temp_v0) {
    case 1:
        D_80137584 = 0x14;
        break;
    case 0:
        break;
    case -1:
        ovl_15_func_80134724();
        D_80137584 = 0x12;
        break;
    }
}
