#include "common.h"
#include "game_types.h"

s32 GetVal8005E5B4(void);
void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_80117F14(s32 arg0);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
s16 ovl_11_func_8011D98C(StructD548 arg0);

s32 ovl_11_func_80117BB4(void) {
    s32 cost;
    s32 message;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D79C) {
    case 0:
        ovl_11_func_80117F14(0);
        D_8012D79C = 5;
        D_8012D7A0 = 1;
        D_80128260 = 0;
        return 0;
    case 1:
        D_8012D52C = 4;
        D_8012D79C = 2;
        D_8012D524 = 0;
        D_8012D528 = 0;
        return 0;
    case 2:
        switch (ovl_11_func_8011B840(&D_8012D524, 0, D_8012D7B8)) {
        case 0:
            break;
        case 1:
            D_80128260 = ovl_11_func_8011D98C(*(StructD548 *)D_8012D548);
            cost = ovl_11_func_800D6014(*(u16 *)&D_80128260);
            if (ovl_11_func_8011D06C(cost) == 1) {
                ovl_11_func_800F2354(-cost, 1);
                D_8012D79C = 5;
                D_8012D7A0 = 4;
                D_8012D52C = 0;
                message = 3;
            } else {
                D_80128260 = 0;
                D_8012D79C = 5;
                D_8012D7A0 = 3;
                message = 1;
            }
            ovl_11_func_80117F14(message);
            break;
        case 2:
            break;
        case 3:
            D_8012D79C = 5;
            D_8012D7A0 = 6;
            D_8012D52C = 0;
            break;
        }
        return 0;
    case 3:
        ovl_11_func_80117F14(2);
        D_8012D79C = 5;
        D_8012D7A0 = 1;
        return 0;
    case 4:
        if (D_8012D7A8[3] != 0x118) {
            if (D_80128260 == 0xAB) {
                D_8012D7A8[4] = 0x200;
            } else {
                D_8012D7A8[4] = 0x131;
            }
        }
        ovl_11_func_80117F14(4);
        D_8012D79C = 5;
        D_8012D7A0 = 6;
        return 0;
    case 5:
        if (GetVal8005E5B4() == 2) {
            D_8012D79C = D_8012D7A0;
        }
        return 0;
    case 6:
    default:
        if (D_80128260 != 0) {
            if ((((Ovl11CBB9CGlobals *)D_8006C838)->items[D_80128260].u0.flags & 0x400) != 0) {
                ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, D_80128260, 3);
            } else {
                ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11CBB9CGlobals *)D_8006C838)->object, D_80128260, 2);
            }
        }
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        return 1;
    }
    return 0;
}
