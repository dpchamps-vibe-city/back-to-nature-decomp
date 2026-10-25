#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_80132EE0(s32 arg0);

s32 ovl_30_func_80133218(s32 arg0) {
    s32 i;
    s32 value;

    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x2000) {
        D_80134ABC++;
        if (D_80134ABC >= 5) {
            D_80134ABC = 0;
        }
    }
    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x8000) {
        D_80134ABC--;
        if (D_80134ABC < 0) {
            D_80134ABC = 4;
        }
    }
    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x1000) {
        D_80134AC0--;
        if (D_80134AC0 < 0) {
            D_80134AC0 = 4;
        }
    }
    if (((SomeStruct *)gPad0Input)->field_0x0 & 0x4000) {
        D_80134AC0++;
        if (D_80134AC0 >= 5) {
            D_80134AC0 = 0;
        }
    }
    FntPrint(D_8012EF2C);
    FntPrint(D_8012EF90, D_80134ABC, D_80134AC4[D_80134ABC]);
    for (i = 0; i < 5; i++) {
        if (i == D_80134AC0) {
            value = ovl_30_func_80132EE0(D_801231C0[D_80134ABC][i]);
            D_801231C0[D_80134ABC][i] = value;
            FntPrint(D_8012DF2C);
        } else {
            FntPrint(D_8012DF30);
        }
        FntPrint(D_8012EF9C, D_801231C0[D_80134ABC][i]);
    }
    return arg0;
}
