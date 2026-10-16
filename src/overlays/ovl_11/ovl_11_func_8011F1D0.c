#include "common.h"

s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_8011F4F4(void);

s32 ovl_11_func_8011F1D0(s32 arg0) {
    u8 *state;
    s32 value;
    s16 base;
    s16 mode;
    s32 count0;
    s32 count1;
    s32 result;
    u8 selected;
    u8 *counter0;
    u8 *counter1;

    state = &D_8007AFE4.states[arg0];
    value = ((struct Ovl11StateSelection *) D_8006C838)->states[arg0];
    base = D_8012852C[arg0];
    mode = ((struct Ovl11StateSelection *) D_8006C838)->mode;
    if (value != 0) {
        switch (arg0) {
        case 0:
            if (func_8001AF44(0x64) == 1) {
                *state = (u8) (value == 2 ? 1 : 2);
                value = *state;
            } else {
                *state = 1;
            }
            break;
        case 1:
            if (func_8001AF44(0x63) == 1) {
                *state = (u8) (value == 2 ? 1 : 2);
                value = *state;
            } else {
                *state = 1;
            }
            break;
        case 2:
            if (func_8001AF44(0x65) == 1) {
                *state = (u8) (value == 2 ? 1 : 2);
                value = *state;
            } else {
                *state = 1;
                value = 1;
            }
            break;
        case 3:
            if (func_8001AF44(0x66) == 1) {
                *state = (u8) (value == 5 ? 3 : value + 1);
                value = 6 - *state;
            } else if (ovl_11_func_8011F4F4() >= 10) {
                *state = (u8) (value == 2 ? 1 : 2);
                value = *state;
            } else {
                *state = 1;
            }
            break;
        case 4: {
            s32 increment;

            increment = 2;
            if (value == increment) {
                *state = 1;
            } else {
                increment = value + 1;
                *state = increment;
            }
            break;
        }
        case 5: {
            s32 increment;

            increment = 5;
            if (value == increment) {
                *state = 1;
            } else {
                increment = value + 1;
                *state = increment;
            }
            break;
        }
        case 6: {
            s32 selected;
            u8 next;

            if (mode == 0) {
                value = 5;
                break;
            }
            selected = 3;
            if (mode == 2) {
                value = 2;
                break;
            }
            switch (value) {
            case 1:
                *state = selected;
                break;
            case 3:
                next = 4;
                *state = next;
                break;
            case 4:
            default:
                *state = 1;
                break;
            }
            break;
        }
        case 7: {
            s32 increment;

            increment = 7;
            if (value == increment) {
                *state = 1;
            } else {
                increment = value + 1;
                *state = increment;
            }
            break;
        }
        case 8: {
            s32 increment;

            increment = 3;
            if (value == increment) {
                *state = 1;
            } else {
                increment = value + 1;
                *state = increment;
            }
            break;
        }
        case 9:
            count0 = ((struct Ovl11StateSelection *) D_8006C838)->counters[0];
            count1 = ((struct Ovl11StateSelection *) D_8006C838)->counters[1];
            counter0 = &D_8007AFEE.first;
            result = func_8001AF44(0x13D);
            counter1 = &D_8007AFE4.second;
            if (result == 1 || func_8001AF44(0x143) == 1) {
                if (count0 >= 2) {
                    *state = (u8) (value == 4 ? 3 : 4);
                } else {
                    *counter0 = (u8) (*counter0 + 1);
                    selected = 3;
                    goto store_selected;
                }
                value = *state;
            } else if (((struct Ovl11StateSelection *) D_8006C838)->threshold > 0xD6D7) {
                if (count1 >= 2) {
                    *state = (u8) (value == 6 ? 5 : 6);
                } else {
                    *counter1 = (u8) (*counter1 + 1);
                    selected = 5;
store_selected:
                    *state = selected;
                }
                value = *state;
            } else {
                *state = (u8) (value == 2 ? 1 : 2);
            }
            break;
        }
    } else {
        result = 1;
        *state = result;
    }
    return value + base;
}
