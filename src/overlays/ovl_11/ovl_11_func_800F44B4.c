#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F44B4(u16 arg0, s32 *arg1, s16 *arg2, s32 arg3) {
    Ovl11DispatchRecord *record;

    switch (arg0) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        record = ((struct Ovl11DispatchState *) D_8006C838)->list10 + arg3;
        break;
    case 0x65:
        record = ((struct Ovl11DispatchState *) D_8006C838)->fixed + 1;
        break;
    case 0x64:
        record = ((struct Ovl11DispatchState *) D_8006C838)->fixed;
        break;
    default:
        return 0;
    }
    arg1[0] = record->x;
    arg1[1] = record->y;
    arg1[2] = record->z;
    *arg2 = record->id;
    return record->flags & 1;
}
