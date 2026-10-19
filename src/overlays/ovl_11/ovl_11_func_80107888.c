#include "common.h"

s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_80107888(s32 arg0, s32 arg1) {
    /* Keep the guard definition nested: its compilation also changes cc1's
     * address expansion in the enclosing function (cse_not_expected). */
    auto s32 nested_ovl_11_func_8010780C(s32 arg0) __asm__("ovl_11_func_8010780C");

    s32 temp_v0;
    s32 var_s0;
    s32 var_v0;

    s32 nested_ovl_11_func_8010780C(s32 index) {
        char *base;
        s32 callRet;

        base = (char *)&D_8006C838;
        if (*(s16 *)(base + 0x44BA) == 3) {
            if (*(s16 *)(base + 0x44BC) == 0x17) {
                callRet = func_8001AF44(0x1B);
                if (callRet == 1) {
                    if (func_8001AF44((index + 0x21) & 0xFFFF) == 0) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }

    switch (arg0) {
    case 0:
        if (func_8001AF44(0x20U) == 0 && func_8001AF44(0x25U) == 0) {
            var_s0 = 0;
        } else {
            var_s0 = 1;
        }
        break;
    case 1:
        if (func_8001AF44(0x26U) == 0 && func_8001AF44(0x2AU) == 0) {
            var_s0 = 0;
        } else {
            var_s0 = 1;
        }
        break;
    case 2:
        var_s0 = 0;
        if ((func_8001AF44(0x184U) == 1) && (func_8001AF44(0x185U) == 0)) {
            var_s0 = func_8001AF44(0x186U) == 0;
        }
        break;
    case 3:
        var_s0 = func_8001AF44(0x27);
        break;
    case 4:
        var_s0 = func_8001AF44(0x26);
        break;
    case 5:
        var_s0 = D_80070CF6 == 0;
        break;
    case 6:
        var_s0 = func_8001AF44(0x20);
        break;
    case 7: {
        char *base;
        base = (char *)&D_8006C838;
        var_s0 = 0;
        if ((*(s16 *) (base + 0x44BA)) == 2) {
            var_s0 = (*(s16 *) (base + 0x44BC)) == 4;
        }
        break;
    }
    case 8: {
        s32 value;
        var_s0 = 0;
        if (func_8001AF44(0x74U) == 1) {
            value = Ovl11Item_80071A8A.id;
            value -= 0x106;
            var_s0 = (u32)value < 2U;
        }
        break;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        var_s0 = nested_ovl_11_func_8010780C(arg0 - 9);
        break;
    case 14: {
        char *base;
        base = (char *)&D_8006C838;
        var_s0 = 0;
        if ((*(s16 *) (base + 0x44BA)) == 2) {
            var_s0 = (*(s16 *) (base + 0x44BC)) == 2;
        }
        break;
    }
    case 15: {
        char *base;
        base = (char *)&D_8006C838;
        var_s0 = 0;
        if ((*(s16 *) (base + 0x44BA)) == 0) {
            var_s0 = (*(s16 *) (base + 0x44BC)) == 0;
        }
        break;
    }
    case 16:
        var_s0 = func_8001AF44(0x1C);
        break;
    case 17:
        var_s0 = func_8001AF44(0x1D);
        break;
    case 18:
        var_s0 = func_8001AF44(0x1F);
        break;
    case 19: {
        Ovl11GuardState *state;
        Ovl11GuardState *timeState;
        s16 *savedArea;
        s16 *savedRoom;
        var_s0 = 0;
        state = (Ovl11GuardState *)D_8006C838;
        savedArea = &state->savedArea;
        if (*savedArea == state->area) {
            state = (Ovl11GuardState *)D_8006C838;
            savedRoom = &state->savedRoom;
            if (*savedRoom == state->room) {
                if ((state->item == 0x103) || (ovl_11_func_800E6EE0(0x103, 1, 1, 0) == 1)) {
                    arg1 = 0xA;
                } else {
                    arg1 = 0x12;
                }
                timeState = (Ovl11GuardState *)D_8006C838;
                var_v0 = timeState->time < arg1;
                if (var_v0 != 0) {
                    var_s0 = 1;
                }
            }
        }
        break;
    }
    case 20:
        var_s0 = 0;
        temp_v0 = func_8001AF44(0x19BU);
        if (temp_v0 == 1) {
            if (D_80070CF6 != temp_v0) {
                var_s0 = D_80070CF6 != 3;
            }
        }
        break;
    case 21:
        var_s0 = func_8001AF44(0x2A);
        break;
    default:
        var_s0 = 0;
        break;
    }
    if (var_s0 == 1) {
        D_8012D038 = arg0;
    }
    return var_s0;
}
