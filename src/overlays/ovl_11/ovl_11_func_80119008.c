#include "common.h"

void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
void func_80022738(void);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800D6090(u16 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B784(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);

s32 ovl_11_func_80119008(void) {
    s32 result;
    s16 price;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = ovl_11_func_800D6014(0xA4);
    result = 0;
    switch (D_8012D524) {
    case 0:
        D_8012D51C = ((struct Ovl11E8C14State *)D_8006C838)->value;
        ovl_11_func_8011D084(1, 0);
    case 1:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x10000000) {
            if (D_8012D51C == 0xA4) {
                func_8002261C(2, 0xBC);
                ovl_11_func_8011D084(2, 4);
            } else if (ovl_11_func_800D6090((u16)D_8012D51C) == 1) {
                func_8002261C(2, 0xB6);
                SetVal8005E334(0);
                ovl_11_func_8011D084(3, 5);
            } else {
                func_8002261C(2, 0xB9);
                ovl_11_func_8011D084(2, 4);
            }
        } else {
            func_8002261C(2, 0xB8);
            ovl_11_func_8011D084(2, 4);
        }
        break;
    case 2:
        if (GetVal8005E5B4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        if (GetVal8005E5B4() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 5:
        D_8012D52C = 2;
        ovl_11_func_8011D084(6, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 6:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(price) == 1) {
                func_80022738();
                func_8002261C(2, 0xBB);
                ovl_11_func_8011D084(2, 7);
            } else {
                func_80022738();
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(2, 4);
            }
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0xB7);
            ovl_11_func_8011D084(2, 4);
            break;
        }
        break;
    case 7:
        D_8012D52C = 0;
        ovl_11_func_800F2354(-price, 1);
        ovl_11_func_800E6EE0(D_8012D51C, 1, 0, 1);
        ovl_11_func_8011D084(8, 4);
        break;
    case 8:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x4000000) {
            ovl_11_func_8011D084(10, 0);
        }
        break;
    case 10:
    case 11:
        if (ovl_11_func_8011B784(&D_8012D524, 10, D_8012D51C)) {
            ovl_11_func_8011D084(4, 0);
        }
        break;
    case 4:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
