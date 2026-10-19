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
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
u16 ovl_11_func_801164F8(void);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);

s32 ovl_11_func_80115FE0(void) {
    Ovl11D8012D7B8 first;
    Ovl11D8012D7B8 second;
    s32 result;
    s16 price;

    first.unk4 = 2;
    first.unk6 = 0xDD;
    first.unk8 = 2;
    first.unkA = 0xDE;
    first.unk10 = 4;
    first.unk0 = D_8012D548;
    second.unk4 = 2;
    second.unk6 = 0xE6;
    second.unk8 = 2;
    second.unkA = 0xFC;
    second.unk10 = 6;
    second.unk0 = D_8012D5A8;
    price = ovl_11_func_800D6014(ovl_11_func_8011D98C(*(StructD548 *)D_8012D5A8));
    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(2, 0);
        break;
    case 1:
        first.unk4 = -1;
        ovl_11_func_8011D084(2, 0);
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        switch (ovl_11_func_8011B840(&D_8012D524, 2, first)) {
        case 0:
            break;
        case 3:
            ovl_11_func_8011D084(0x1A, 0);
            break;
        case 1:
            switch (((StructD548 *)D_8012D548)->index) {
            case 0:
                func_800226D8(0);
                func_8002261C(2, 0xE5);
                ovl_11_func_8011D084(0x19, 9);
                break;
            case 1:
                ovl_11_func_8011D084(0x10, 0);
                break;
            }
            break;
        case 2:
            break;
        }
        break;
    case 9:
        switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            func_80022738();
            if (ovl_11_func_8011D06C(10) == 1) {
                ovl_11_func_800F2354(-10, 1);
                func_8002261C(2, 0xE7);
                ovl_11_func_8011D084(0x18, 10);
            } else {
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(0x18, 1);
            }
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0xE8);
            ovl_11_func_8011D084(0x18, 1);
            break;
        }
        break;
    case 10:
        func_800132F0(20, 1, 2);
        ovl_11_func_8011D084(11, 0);
        break;
    case 11:
        if (func_80013394() == 1) {
            func_8002261C(2, 0xE9);
            ovl_11_func_8011D084(0x18, 12);
        }
        break;
    case 12:
        func_800132B8(20, 1, 2);
        ovl_11_func_8011D084(13, 0);
        break;
    case 13:
        if (func_80013394() == 1) {
            func_8002261C(2, 0xEA);
            ovl_11_func_8011D084(0x18, 14);
        }
        break;
    case 14:
        func_8002261C(2, ovl_11_func_801164F8());
        ovl_11_func_8011D084(0x18, 0x1A);
        break;
    case 15:
        second.unk4 = -1;
        ovl_11_func_8011D084(0x10, 0);
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        switch (ovl_11_func_8011B840(&D_8012D524, 0x10, second)) {
        case 0:
            break;
        case 3:
            second.unk8 = -1;
            ovl_11_func_8011D084(1, 0);
            break;
        case 1:
            if (ovl_11_func_8011D06C(price) == 1) {
                ovl_11_func_800F2354(-price, 1);
                func_8002261C(2, 0xFB);
                ovl_11_func_8011D084(0x18, 0x1B);
            } else {
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(0x18, 0x17);
            }
            break;
        case 2:
            break;
        }
        break;
    case 23:
        func_8002261C(2, 0xFC);
        ovl_11_func_8011D084(0x18, 1);
        break;
    case 25:
        if (func_800226A4() == 5) {
            func_80017B18(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 24:
        if (func_800226A4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 26:
        result = 1;
        break;
    case 27:
    case 28:
        if (ovl_11_func_8011B6C4(&D_8012D524, 27, ((StructD548 *)D_8012D5A8)->data[((StructD548 *)D_8012D5A8)->index])) {
            ovl_11_func_8011D084(26, 0);
        }
        break;
    }
    return result;
}
