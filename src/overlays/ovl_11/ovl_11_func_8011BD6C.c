#include "common.h"
#include "game_types.h"

void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
void func_800132F0(s32 arg0, s32 arg1, s32 arg2);
s32 func_80013394(void);
void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
void func_800226D8(s32 arg0);
void func_80022738(void);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800E5E98(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9284(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_80111E68(s16 arg0);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D098(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);

s32 ovl_11_func_8011BD6C(s32 *arg0, s32 arg1, StructD548 **arg2) {
    s16 messages[4][2];
    s32 state;
    s32 valid;
    s32 result;
    s16 item;
    s16 mode;

    state = *arg0 - arg1;
    if ((u32)state >= 15U) {
        return -1;
    }
    item = ovl_11_func_8011D98C(**arg2);
    valid = (*arg2)->index != 1;
    if (valid) {
        mode = 0x62;
        messages[0][0] = 2;
        messages[0][1] = 0x1E2;
        messages[1][0] = 2;
        messages[1][1] = 0xA4;
        messages[2][0] = 2;
        messages[2][1] = 0xA4;
        messages[3][0] = 2;
        messages[3][1] = 0xA5;
    } else {
        mode = 0x61;
        messages[0][0] = 2;
        messages[0][1] = 0x1E1;
        messages[1][0] = 2;
        messages[1][1] = 0xA3;
        messages[2][0] = 2;
        messages[2][1] = 0xA3;
        messages[3][0] = 2;
        messages[3][1] = 0x9E;
    }
    result = 0;
    if ((u32)state < 15U) {
        switch ((u32)state) {
        case 0:
            if (ovl_11_func_800E6EE0(mode, 1, 1, 0) == 1) {
                if (ovl_11_func_8011D06C(ovl_11_func_800D6014(item)) == 1) {
                    func_800226D8(0);
                    func_8002261C(messages[0][0], messages[0][1]);
                    ovl_11_func_8011D098(2, 4, arg1);
                } else {
                    func_8002261C(messages[1][0], messages[1][1]);
                    ovl_11_func_8011D098(1, 3, arg1);
                }
            } else {
                func_8002261C(messages[2][0], messages[2][1]);
                ovl_11_func_8011D098(1, 3, arg1);
            }
            break;
        case 1:
            if (func_800226A4() == 2) {
                ovl_11_func_8011D0B4();
            }
            break;
        case 2:
            if (func_800226A4() == 5) {
                func_80017B18(0);
                ovl_11_func_8011D0B4();
            }
            break;
        case 13:
            if (D_80071A6C & 0x10000000) {
                ovl_11_func_8011D0B4();
            }
            break;
        case 4:
            ovl_11_func_8011D098(5, 0, arg1);
            ovl_11_func_80118B84(0, -1, 0);
        case 5:
            switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 200, 0x80)) {
            case 0:
                break;
            case 1:
                func_80022738();
                func_800226D8(0);
                func_8002261C(messages[3][0], messages[3][1]);
                ovl_11_func_8011D098(2, 6, arg1);
                break;
            case 2:
                func_80022738();
                func_8002261C(2, 0x93);
                ovl_11_func_8011D098(1, 3, arg1);
                break;
            }
            break;
        case 6:
            ovl_11_func_8011D098(7, 0, arg1);
            ovl_11_func_80118B84(0, -1, 0);
        case 7:
            switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 0xE0, 0x80)) {
            case 0:
                break;
            case 1:
                func_80022738();
                ovl_11_func_8011D098(8, 0, arg1);
                break;
            case 2:
                func_80022738();
                func_8002261C(2, 0x93);
                ovl_11_func_8011D098(1, 3, arg1);
                break;
            }
            break;
        case 8:
            D_8012D52C = 0;
            ovl_11_func_800E6EE0(mode, 1, 1, 1);
            ovl_11_func_800E6F34(mode, 0, 0, 0);
            ovl_11_func_8011D098(13, 14, arg1);
            break;
        case 14:
            ovl_11_func_800F2354(-ovl_11_func_800D6014(item), 1);
            ovl_11_func_80111E68(item);
            func_80022738();
            func_8002261C(2, 0x9F);
            ovl_11_func_8011D098(1, 12, arg1);
            break;
        case 12:
            ovl_11_func_800E6EE0(mode, 1, 0, 1);
            func_800132F0(20, 0, 2);
            ovl_11_func_8011D098(10, 0, arg1);
        case 10:
            if (func_80013394() == 1) {
                ovl_11_func_800E5E98(32, 0, 0, 0);
                ((Ovl11A25CState *)D_8006C838)->x = 1000;
                ((Ovl11A25CState *)D_8006C838)->z = -2800;
                ((Ovl11A25CState *)D_8006C838)->y = 0;
                ovl_11_func_800E9284(3, 0, 0, 0);
                func_800132B8(20, 0, 2);
                ovl_11_func_8011D098(11, 0, arg1);
            }
            break;
        case 11:
            if (func_80013394() != 1) {
                break;
            }
            ovl_11_func_8011D098(9, 0, arg1);
        case 9:
            ovl_11_func_800E9B68(4, 0, 0, 0);
            result = 1;
            break;
        case 3:
            result = 3;
            break;
        }
    }
    return result;
}
