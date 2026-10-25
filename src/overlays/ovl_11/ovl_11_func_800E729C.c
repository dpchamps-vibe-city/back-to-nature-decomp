#include "common.h"
#include "game_types.h"
#include "text_entry.h"

s32 ovl_11_func_800E729C(s16 arg0, s16 arg1, s32 arg2) {
    s32 result;
    s32 temp;

    result = 0;
    temp = UpdateTextEntry();
    switch (temp) {
    case -1:
        if (arg2 != temp) {
            gOvl11ValueSlots[arg2] = 1;
        }
        result = 1;
        break;
    case 1:
        if (arg2 != -1) {
            gOvl11ValueSlots[arg2] = 0;
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
            BeginRequiredTextEntry((u16 *)&D_801295D0);
            break;
        case 1:
        case 2:
        case 3:
            if (arg1 == 0) {
                BeginTextEntry((u16 *)&D_801295D0);
            } else {
                BeginRequiredTextEntry((u16 *)&D_801295D0);
            }
            break;
        }
        break;
    }
    return result;
}
