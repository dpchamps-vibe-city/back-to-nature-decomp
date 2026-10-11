#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 func_80011F5C(s32 arg0);
void func_80011FD8(s32 arg0);

void ovl_28_func_800B8E9C(u_long *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9, u8 arg10) {
    POLY_G4 *temp_v0;
    s32 sp10;
    s32 sp14;
    s32 right;
    s32 bottom;

    sp10 = arg3;
    sp14 = arg10;
    temp_v0 = (POLY_G4 *)func_80011F5C(0);
    setPolyG4(temp_v0);
    setSemiTrans(temp_v0, arg9);
    right = arg1 + sp10;
    bottom = arg2 + arg4;
    setXY4(temp_v0, arg1, arg2, right, arg2, arg1, bottom, right, bottom);
    setRGB0(temp_v0, arg5 >> 16, arg5 >> 8, arg5);
    setRGB1(temp_v0, arg6 >> 16, arg6 >> 8, arg6);
    setRGB2(temp_v0, arg7 >> 16, arg7 >> 8, arg7);
    setRGB3(temp_v0, arg8 >> 16, arg8 >> 8, arg8);
    addPrim(arg0, temp_v0);
    temp_v0++;
    setDrawTPage((DR_TPAGE *)temp_v0, 0, 1, getTPage(0, sp14, 0, 0));
    addPrim(arg0, temp_v0);
    func_80011FD8((s32)((DR_TPAGE *)temp_v0 + 1));
}
