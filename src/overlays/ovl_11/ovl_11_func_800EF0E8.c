#include "common.h"
#include "game_types.h"
#include "game_flags.h"

s32 ovl_11_func_800EF8BC(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800EFA1C(s16 arg0);

s32 ovl_11_func_800EF0E8(s16 arg0) {
    u32 first;
    u32 second;
    s16 *state;
    Ovl11EF488Record *record;

    first = 0;
    second = 0;
    state = (s16 *) D_80070CF0_halfwords;
    if (ovl_11_func_800EF8BC(arg0, state[1], state[2]) == 0) {
        return 0;
    }
    if (arg0 != -1) {
        record = &D_8007AD10_records[arg0];
    } else {
        record = (Ovl11EF488Record *) ((u8 *) state + 0xFD0);
    }
    if (record->index_8 == -1) {
        return 0;
    }
    switch (arg0) {
    case -1:
        if (GetGameFlag(0x1B) == 0) {
            return 0;
        }
    case 4:
        if (GetGameFlag(0x122) != 0) {
            return 0;
        }
        if (GetGameFlag(0x2A) == 1) {
            return 1;
        }
        if (ovl_11_func_800EFA1C(0) == 1) {
            return 0;
        }
        break;
    default:
        switch (arg0) {
        case 0:
            first = 0x21;
            second = 0x26;
            break;
        case 1:
            first = 0x22;
            second = 0x27;
            break;
        case 2:
            first = 0x23;
            second = 0x28;
            break;
        case 3:
            first = 0x24;
            second = 0x29;
            break;
        }
        if (GetGameFlag(second) == 1) {
            return 1;
        }
        if (GetGameFlag(first) == 1) {
            return 0;
        }
        break;
    }
    if (arg0 != 4) {
        if ((s16) record->field_0 > state[0]) {
            return 0;
        }
        if ((s16) record->field_0 == state[0] && (s16) record->field_2 > state[1]) {
            return 0;
        }
        if (record->field_2 == (u16) state[1] && (s16) record->field_4 > state[2]) {
            return 0;
        }
    } else {
        if ((s16) record->field_0 > state[0]) {
            return 0;
        }
        if ((s16) record->field_2 != state[1]) {
            return 0;
        }
        if ((s16) record->field_4 != state[2]) {
            return 0;
        }
    }
    if (arg0 == -1) {
        return 1;
    }
    if (arg0 != 4) {
        if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_A * 0x1D4))->field_99EC > 0xEA5F &&
            ((Ovl11EF488SelectionView *) D_8006C838)->selected != -1) {
            return 0;
        }
        if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_8 * 0x1D4))->field_99EA >= 200) {
            return 2;
        }
        if (arg0 == 0) {
            if (((Ovl11EF488SelectionView *) D_8006C838)->selected == 20) {
                if (GetGameFlag(0x1B) == 0) {
                    return 2;
                } else {
                    return 1;
                }
            }
        }
    } else {
        if (GetGameFlag(0xC9) == 1) {
            if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_A * 0x1D4))->field_99EC > 0xEA5F &&
                ((Ovl11EF488SelectionView *) D_8006C838)->selected != -1) {
                return 0;
            }
            return 1;
        }
        if (GetGameFlag(0x11E) == 1) {
            return 2;
        }
    }
    return 1;
}
