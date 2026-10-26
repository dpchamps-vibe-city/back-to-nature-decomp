#include "common.h"

s32 ovl_15_func_80133808(s16 arg0);
void ovl_15_func_80134724(void);

void ovl_15_func_80131FD0(void) {
    s32 result;

    result = ovl_15_func_80133808(0);
    switch (result) {
    case 0:
        break;
    case 1:
        ovl_15_func_80134724();
        gOvl15MenuState = 0x13;
        break;
    case -1:
        gOvl15MenuState = 9;
        break;
    }
}
