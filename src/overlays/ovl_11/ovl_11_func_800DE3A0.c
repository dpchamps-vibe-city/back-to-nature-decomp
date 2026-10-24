#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 func_8001AF44 (u32 arg0);
void ovl_11_func_800E3C04 (void);
void ovl_11_func_800DBAB0 (void);
void ovl_11_func_800F1038 (u16 arg0, s32 arg1);
s32 ovl_11_func_800F0F50 (void);
void func_8001AF70 (u16 arg0, u16 arg1);
s32 ovl_11_func_80118C6C (void);
s32 ovl_11_func_800DBEF8(s32 arg0);
s32 ovl_11_func_800DBE30 (s32 arg0);
s32 ovl_11_func_800E7798 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F1078 (Ovl11ConditionRecord *arg0, s32 arg1);
s32 ovl_11_func_800F1354 (s32 arg0, s32 arg1);

void ovl_11_func_800DE3A0(void) {
    if (func_8001AF44(2U) == 1) {
        ovl_11_func_800E3C04();
        if (D_800742AC == 2) {
            ovl_11_func_800DBAB0();
        }
    }
    ovl_11_func_800F1038(0x12DU, 0);
    ovl_11_func_800F0F50();
    ovl_11_func_800DBAB0();
    if (func_8001AF44(2U) == 1) {
        do {
            if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                ovl_11_func_800DBAB0();
                break;
            }
            ovl_11_func_800DBAB0();
        } while (func_8001AF44(2U) != 0);
    }
}
