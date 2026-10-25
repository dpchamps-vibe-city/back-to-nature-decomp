#include "common.h"
#include "game_flags.h"

void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
void ovl_11_func_800DBAB0(void);

s32 ovl_11_func_80118A68(s32 arg0) {
    s32 (*handler)(void);
    s32 result = 1;

    if ((D_8012D520 == 0) || (arg0 == 1)) {
        if ((arg0 == 0x16) && (GetGameFlag(2U) == 0)) {
            ovl_11_func_800F0FB0(0xA);
            ovl_11_func_800F0F50();
            ovl_11_func_800DBAB0();
            if (GetGameFlag(2U) == 1) {
                do {
                    if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                        ovl_11_func_800DBAB0();
                        break;
                    }
                    ovl_11_func_800DBAB0();
                } while (GetGameFlag(2U) != 0);
            }
            if (GetGameFlag(2U) == 1) {
                return 0;
            }
        }
        handler = D_800BB2BC[arg0];
        D_8012D520 = arg0;
        result = 0;
        if (handler != 0) {
            handler();
        }
    }
    return result;
}
