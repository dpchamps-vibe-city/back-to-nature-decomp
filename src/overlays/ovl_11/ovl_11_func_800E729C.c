#include "common.h"
#include "game_types.h"

s32 func_800231E8(void);
void func_80023170(u16 *arg0);
void func_800231AC(u16 *arg0);

s32 ovl_11_func_800E729C(s16 arg0, s16 arg1, s32 arg2) {
    s32 result;
    s32 temp;

    result = 0;
    temp = func_800231E8();
    switch (temp) {
    case -1:
        if (arg2 != temp) {
            D_80129560[arg2] = 1;
        }
        result = 1;
        break;
    case 1:
        if (arg2 != -1) {
            D_80129560[arg2] = 0;
        }
        switch (arg0) {
        case 0:
            *(Ovl11Status801295D0 *)&D_80075AD8 = D_801295D0;
            break;
        case 4:
            *(Ovl11Status801295D0 *)&D_8007AFBC = D_801295D0;
            break;
        }
        result = 1;
        break;
    case 0:
        switch (arg0) {
        case 0:
        case 4:
            func_80023170((u16 *)&D_801295D0);
            break;
        case 1:
        case 2:
        case 3:
            if (arg1 == 0) {
                func_800231AC((u16 *)&D_801295D0);
            } else {
                func_80023170((u16 *)&D_801295D0);
            }
            break;
        }
        break;
    }
    return result;
}
