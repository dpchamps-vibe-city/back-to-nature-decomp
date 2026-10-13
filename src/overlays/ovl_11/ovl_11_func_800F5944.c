#include "common.h"
#include "game_types.h"

void ovl_11_func_800F5BC0(s32 arg0, M2C_0584a699a1f0_Ovl11Func5BC0Entry *arg1, s32 (*arg2)(u16, u8 *, s32 *));
s32 ovl_11_func_800F5D04(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F5EE0(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F6118(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F6218(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F62D8(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F64F8(s32 arg0, s32 *arg1, s32 *arg2);
s32 ovl_11_func_800F6578(s32 arg0, s32 *arg1, s32 *arg2);

void ovl_11_func_800F5944(void) {
    Ovl11DispatchRecord record;
    s16 selector;
    s16 mode;
    s32 (*callback)(s32, s32 *, s32 *);
    s32 index;

    mode = ((struct Ovl11DispatchWork *) &D_8007AFF0)->header[5];
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
        callback = ovl_11_func_800F6118;
        break;
    case 4:
        callback = ovl_11_func_800F64F8;
        break;
    case 5:
        callback = ovl_11_func_800F5D04;
        break;
    case 6:
        callback = ovl_11_func_800F62D8;
        break;
    case 7:
        callback = ovl_11_func_800F6218;
        break;
    case 8:
        callback = ovl_11_func_800F5EE0;
        break;
    case 9:
        callback = ovl_11_func_800F6578;
        break;
    default:
        return;
    }

    selector = ((struct Ovl11DispatchWork *) &D_8007AFF0)->selector - 1;
    switch (selector) {
    case 0:
        index = 0;
        break;
    case 1:
        index = 1;
        break;
    case 3:
        index = 3;
        break;
    case 4:
        index = 2;
        break;
    case 0x2F:
        index = 4;
        break;
    case 7:
        index = 5;
        break;
    case 0x13:
        index = 7;
        break;
    case 0x19:
        index = 9;
        break;
    case 0x1C:
        index = 8;
        break;
    case 0x1D:
        index = 0xA;
        break;
    case 0x21:
        index = 6;
        break;
    case 0x33:
        index = 0xB;
        break;
    case 0xB:
        index = 0xC;
        break;
    case 0xC:
        index = 0xD;
        break;
    case 0xD:
        index = 0xE;
        break;
    case 0xE:
        index = 0xF;
        break;
    default:
        index = -1;
        break;
    }

    if (index >= 0) {
        ovl_11_func_800F5BC0(
            ((struct Ovl11DispatchState *) D_8006C838)->counts[index],
            (M2C_0584a699a1f0_Ovl11Func5BC0Entry *) ((struct Ovl11DispatchState *) D_8006C838)->lists[index],
            (s32 (*)(u16, u8 *, s32 *)) callback);
    }
    ovl_11_func_800F5BC0(1,
        (M2C_0584a699a1f0_Ovl11Func5BC0Entry *) (((struct Ovl11DispatchState *) D_8006C838)->fixed + 1),
        (s32 (*)(u16, u8 *, s32 *)) callback);
    record = *((struct Ovl11DispatchState *) D_8006C838)->fixed;
    if (record.flags & 0x100) {
        record.y += D_80126C98[D_80129634];
    }
    ovl_11_func_800F5BC0(1, (M2C_0584a699a1f0_Ovl11Func5BC0Entry *) &record,
        (s32 (*)(u16, u8 *, s32 *)) callback);
    ovl_11_func_800F5BC0(0xA,
        (M2C_0584a699a1f0_Ovl11Func5BC0Entry *) ((struct Ovl11DispatchState *) D_8006C838)->list10,
        (s32 (*)(u16, u8 *, s32 *)) callback);
}
