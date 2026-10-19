#include "common.h"

void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
void func_800226D8(s32 arg0);
void func_80022738(void);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_800F3A44(u16 arg0, u16 arg1);
void ovl_11_func_800F45A4(s16 arg0, s32 arg1);
s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011CF40(s16 arg0, u16 *arg1);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);

s32 ovl_11_func_80118CE4(void) {
    s32 result;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(9, 0);
        ovl_11_func_800F45A4(0x84, 0);
        break;
    case 9:
    case 10:
        if (ovl_11_func_8011B6C4(&D_8012D524, 9, 0x6C)) {
            ovl_11_func_8011D084(1, 0);
        }
        break;
    case 1:
        if (((Ovl11FB908State *)D_8006C838)->field_4506 >= 999) {
            func_8002261C(2, 0xD1);
            ovl_11_func_8011D084(2, 7);
        } else {
            func_8002261C(2, 0xD0);
            ovl_11_func_8011D084(2, 4);
        }
        break;
    case 2:
        if (func_800226A4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        if (func_800226A4() == 5) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 4:
        func_800226D8(0);
        func_80017B18(0);
        func_8002261C(2, 0x4D);
        ovl_11_func_8011D084(3, 5);
        break;
    case 5:
        D_8012D52C = 1;
        switch (ovl_11_func_8011CF40(999, (u16 *)&D_8012D538.unk8)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(D_8012D538.unk8 * D_8012D538.unkA) == 1) {
                if (D_8012D538.unk8 + ((Ovl11FB908State *)D_8006C838)->field_4506 >= 1000) {
                    func_80022738();
                    func_8002261C(2, 0xD2);
                    ovl_11_func_8011D084(2, 4);
                } else {
                    func_80022738();
                    func_8002261C(2, 0xD3);
                    ovl_11_func_8011D084(2, 6);
                }
            } else {
                func_80022738();
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(2, 4);
            }
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0x25B);
            ovl_11_func_8011D084(2, 4);
            break;
        case 3:
            func_80022738();
            func_8002261C(2, 0xBA);
            ovl_11_func_8011D084(2, 7);
            break;
        }
        break;
    case 6:
        ovl_11_func_800F3A44(1, (u16)D_8012D538.unk8);
        ovl_11_func_800F2354(-(D_8012D538.unk8 * D_8012D538.unkA), 1);
    case 7:
        ovl_11_func_800F45A4(0x84, 1);
        ovl_11_func_800E6EE0(0x6C, 1, 0, 1);
        ovl_11_func_8011D084(8, 0);
        break;
    case 8:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
