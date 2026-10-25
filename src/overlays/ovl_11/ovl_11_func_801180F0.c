#include "common.h"
#include "game_types.h"

void SetVal8005E2BC(s32 arg0);
s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022738(void);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_80111EE0(void);
s32 ovl_11_func_80118744(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
s32 ovl_11_func_8011D934(StructD548 arg0);

s32 ovl_11_func_801180F0(void) {
    s32 choice;

    switch (D_8012D7CC) {
    case 0:
        D_8012D7CC = 8;
        D_8012D7D0 = 1;
        func_8002261C(2, 0x132);
        return 0;
    case 1:
        D_8012D7CC = 2;
        D_8012D524 = 0;
        D_8012D528 = 0;
        D_8012D7D8.unk0 = D_8012D548;
        return 0;
    case 2:
        switch (ovl_11_func_8011B840(&D_8012D524, 0, *(Ovl11D8012D7B8 *)&D_8012D7D8)) {
        case 0:
            break;
        case 1:
            D_8012D7CC = 8;
            D_8012D7D0 = 9;
            if (ovl_11_func_8011D934(*(StructD548 *)D_8012D548) == 0) {
                if (D_80070D38 >= 999) {
                    D_8012828C = 0x13A;
                    D_80128290 = 1;
                } else {
                    D_8012828C = 0x13B;
                    D_80128290 = 3;
                }
            } else if (func_8001AF44(0x38) == 1) {
                D_8012828C = 0x144;
                D_80128290 = 1;
            } else if (D_8012D7D4 == -1) {
                D_8012828C = 0x146;
                D_80128290 = 1;
            } else {
                D_8012828C = D_8012D7D4 + 0x13C;
                D_80128290 = 5;
            }
            func_80022738();
            break;
        case 2:
            break;
        case 3:
            D_8012D7CC = 8;
            D_8012D7D0 = 10;
            D_8012D52C = 0;
            break;
        }
        return 0;
    case 3:
        D_8012D7CC = 4;
        D_8012D524 = 0;
        D_8012D528 = 0;
        D_8012D7D8.unk0 = D_8012D5A8;
        return 0;
    case 4:
        switch (ovl_11_func_80118744(&D_8012D524, 0, 5, *(Ovl11BAFCParams *)&D_8012D7D8, (ovl_11_DD48_arg0 *)&D_8012D538)) {
        case 0:
            break;
        case 1:
            D_8012D7CC = 8;
            D_8012D7D0 = 9;
            if (ovl_11_func_8011D06C(D_8012D538.unk8 * D_8012D538.unkA) == 1) {
                if (D_8012D538.unk8 + ((Ovl11ResourceGlobals *)D_8006C838)->amount < 1000) {
                    ovl_11_func_800F2354(-(D_8012D538.unk8 * D_8012D538.unkA), 1);
                    D_8012828C = 0x143;
                    D_80128290 = 10;
                    ((Ovl11ResourceGlobals *)D_8006C838)->amount += D_8012D538.unk8;
                } else {
                    D_8012828C = 0x142;
                    D_80128290 = 3;
                }
            } else {
                D_8012828C = 0x4F;
                D_80128290 = 3;
            }
            func_80022738();
            break;
        case 2:
            break;
        case 3:
            D_8012D7CC = 8;
            D_8012D7D0 = 1;
            break;
        }
        return 0;
    case 5:
        ovl_11_func_80118B84(0, -1, 0);
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(2, 0x5F);
        D_8012D7CC = 6;
        return 0;
    case 6:
        if (GetVal8005E5B4() == 5) {
            D_8012D7CC = 7;
        }
        return 0;
    case 7:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 1:
            choice = 4;
            if (D_8012D7D4 != -1) {
                choice = D_8012D7D4;
            }
            if (ovl_11_func_8011D06C(D_80128278[choice][0]) == 1) {
                if (((Ovl11ResourceGlobals *)D_8006C838)->amount >= D_80128278[choice][1]) {
                    D_8012828C = 0x144;
                    D_8012D7CC = 8;
                    D_8012D7D0 = 9;
                    D_80128290 = 10;
                    ((Ovl11ResourceGlobals *)D_8006C838)->amount -= D_80128278[choice][1];
                    ovl_11_func_800F2354(-D_80128278[choice][0], 1);
                    func_8001AF70(0x38, 1);
                    ovl_11_func_80111EE0();
                } else {
                    D_8012828C = 0x145;
                    D_8012D7CC = 8;
                    D_8012D7D0 = 9;
                    D_80128290 = 1;
                }
            } else {
                D_8012828C = 0x4F;
                D_8012D7CC = 8;
                D_8012D7D0 = 9;
                D_80128290 = 1;
            }
            func_80022738();
            break;
        case 2:
            func_80022738();
            D_8012828C = 0x141;
            D_8012D7CC = 8;
            D_8012D7D0 = 9;
            D_80128290 = 1;
            break;
        }
        return 0;
    case 8:
        if (GetVal8005E5B4() == 2) {
            D_8012D7CC = D_8012D7D0;
        }
        return 0;
    case 9:
        if (D_8012828C != -1) {
            func_8002261C(2, D_8012828C);
        }
        D_8012D7CC = 8;
        D_8012D7D0 = D_80128290;
        return 0;
    case 10:
    default:
        D_8012D52C = 0;
        return 1;
    }
    return 0;
}
