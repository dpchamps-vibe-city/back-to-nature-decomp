#include "common.h"

void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
void func_80022738(void);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800E7088(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_800F45A4(s16 arg0, s32 arg1);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s32 ovl_11_func_8011D35C(s32 arg0, s16 arg1, s16 arg2);
s16 ovl_11_func_8011D9DC(s16 arg0, s32 arg1);

s32 ovl_11_func_80119820(void) {
    s32 result;
    s16 price;

    if (!(((struct Ovl11CallbackGateState *)D_8006C838)->flags & 0x100)) {
        return 1;
    }
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = ovl_11_func_800D6014(0x21);
    result = 0;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(11, 0);
        break;
    case 11:
    case 12:
        func_8002261C(2, 0xDC);
        SetVal8005E334(0);
        ovl_11_func_8011D084(3, 4);
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
    case 4:
        D_8012D52C = 3;
        ovl_11_func_8011D084(8, 0);
        ovl_11_func_8011D35C(0, -1, 0);
    case 8:
        switch (ovl_11_func_8011D35C(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D9DC(0x21, 1) == 0) {
                func_80022738();
                func_8002261C(2, 0x54);
                ovl_11_func_8011D084(2, 7);
            } else if (ovl_11_func_8011D06C(price) == 1) {
                func_80022738();
                func_8002261C(2, 0xBB);
                ovl_11_func_8011D084(2, 6);
            } else {
                func_80022738();
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(2, 7);
            }
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0xBA);
            ovl_11_func_8011D084(2, 7);
            break;
        }
        break;
    case 6:
        ovl_11_func_800F45A4(0x21, 0);
        ovl_11_func_800F2354(-price, 1);
        ovl_11_func_800E7088(0x21, 0, 1, 1);
        ((struct Ovl11CallbackGateState *)D_8006C838)->status |= 0x200000;
        ((struct Ovl11CallbackGateState *)D_8006C838)->flags &= ~0x100;
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    case 7:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
