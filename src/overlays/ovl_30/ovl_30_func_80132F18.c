#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_80132EE0(s32 arg0);

s32 ovl_30_func_80132F18(s32 arg0) {
    ItemData *temp_s0;

    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 8) {
        D_80134AB4 += 10;
        if (D_80134AB4 >= 0x17B) {
            D_80134AB4 = 0;
        }
    }
    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 4) {
        D_80134AB4 -= 10;
        if (D_80134AB4 <= 0) {
            D_80134AB4 = 0x17A;
        }
    }
    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 0x2000) {
        D_80134AB4++;
        if (D_80134AB4 >= 0x17B) {
            D_80134AB4 = 0;
        }
    }
    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 0x8000) {
        D_80134AB4--;
        if (D_80134AB4 <= 0) {
            D_80134AB4 = 0x17A;
        }
    }
    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 0x1000) {
        D_80134AB8--;
        if (D_80134AB8 < 0) {
            D_80134AB8 = 3;
        }
    }
    if (((SomeStruct *)D_8005E3A8)->field_0x0 & 0x4000) {
        D_80134AB8++;
        if (D_80134AB8 >= 4) {
            D_80134AB8 = 0;
        }
    }
    temp_s0 = &D_8006C858[D_80134AB4];
    FntPrint(D_8012EF2C);
    FntPrint(D_8012EF30, D_80134AB4);
    if (D_80134AB8 == 0) {
        FntPrint(D_8012DF2C);
        temp_s0->field_10 = ovl_30_func_80132EE0(temp_s0->field_10);
    } else {
        FntPrint(D_8012DF30);
    }
    FntPrint(D_8012EF3C, temp_s0->field_10);
    if (D_80134AB8 == 1) {
        FntPrint(D_8012DF2C);
        temp_s0->field_12 = ovl_30_func_80132EE0(temp_s0->field_12);
    } else {
        FntPrint(D_8012DF30);
    }
    FntPrint(D_8012EF48, temp_s0->field_12);
    if (D_80134AB8 == 2) {
        FntPrint(D_8012DF2C);
        temp_s0->action.field_4 = ovl_30_func_80132EE0(temp_s0->action.field_4);
    } else {
        FntPrint(D_8012DF30);
    }
    FntPrint(D_8012EF54, temp_s0->action.field_4);
    if (D_80134AB8 == 3) {
        FntPrint(D_8012DF2C);
        temp_s0->action.field_5 = ovl_30_func_80132EE0(temp_s0->action.field_5);
    } else {
        FntPrint(D_8012DF30);
    }
    FntPrint(D_8012EF60, temp_s0->action.field_5);
    return arg0;
}
