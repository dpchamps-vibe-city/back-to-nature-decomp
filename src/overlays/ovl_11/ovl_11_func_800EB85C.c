#include "common.h"
#include "ovl11_state_record_commands.h"
#include "psyq/stddef.h"
#include "ovl11_state_record_motion_limit.h"

s32 ovl_11_func_800EDF3C();

s32 ovl_11_func_800EB85C(s16 arg0) {
    s16 i;
    s16 state;
    s32 result;
    s32 start;

    state = *(s16 *)&gOvl11ValueSlots[arg0];
    start = ovl_11_func_800EDF3C(D_80124984[0], 0, 0, 0) != 1;
    result = 0;
    switch (state) {
    case 0:
        for (i = start; i < 5; i++) {
            Ovl11WriteStateRecordPosition(D_80124984[i], 0, D_80124990[0][i], D_80124990[1][i]);
            Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
        }
        result = 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(D_80124984[i], 10, 0, 0);
        }
        break;
    case 5:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 0, 500, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 2, 2200, 6);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 11);
            }
        }
        break;
    case 6:
        result = Ovl11StateRecordAnimationCommand(D_80124984[1], 12, 0, 0);
        break;
    case 7:
        Ovl11StateRecordAnimationCommand(D_80124984[1], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(D_80124984[3], 12, 0, 0);
        if (result == 1) {
            state += start;
        }
        break;
    case 8:
        Ovl11StateRecordAnimationCommand(D_80124984[3], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(D_80124984[0], 12, 0, 0);
        break;
    case 9:
        Ovl11StateRecordAnimationCommand(D_80124984[3], 5, 0, 1);
        Ovl11StateRecordAnimationCommand(D_80124984[0], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(D_80124984[4], 12, 0, 0);
        break;
    case 10:
        Ovl11StateRecordAnimationCommand(D_80124984[4], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(D_80124984[2], 12, 0, 0);
        break;
    case 11:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 2, 1350, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 0, 1350, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
            }
        }
        break;
    case 12:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(D_80124984[i], 13, 0, 0);
        }
        break;
    case 13:
    case 14:
    case 15:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 0);
        }
        break;
    case 16:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 2, D_80124990[1][i], 8);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(D_80124984[i], 0, D_80124990[1][i], 6);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(D_80124984[i], 5, 0, 1);
            }
        }
        break;
    }
    if (result == 1) {
        if (state == 16) {
            state = 1;
        } else {
            state++;
        }
    }
    gOvl11ValueSlots[arg0] = state;
    return 1;
}
