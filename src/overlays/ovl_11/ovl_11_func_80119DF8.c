#include "common.h"
#include "game_types.h"

s32 func_800226A4(void);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022738(void);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_80111F10(void);
s32 ovl_11_func_8011B2B0(void);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011BD6C(s32 *arg0, s32 arg1, StructD548 **arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 arg0);
s32 ovl_11_func_8011D9DC(s16 arg0, s32 arg1);
s32 ovl_11_func_8011DAD4(s16 id, s16 amount, s32 mode);

s32 ovl_11_func_80119DF8(void) {
    Ovl11D8012D7B8 first;
    Ovl11D8012D7B8 second;
    s32 result;
    s32 flags;
    s16 item;
    s16 price;

    flags = ((struct Ovl11CallbackGateState *)D_8006C838)->flags;
    first.unk4 = -1;
    first.unk6 = 0x85;
    first.unk8 = 2;
    first.unkA = 0x86;
    first.unk10 = 4;
    first.unk0 = D_8012D548;
    second.unk4 = -1;
    second.unk6 = 0;
    second.unk8 = 2;
    second.unkA = 0x91;
    second.unk10 = 6;
    second.unk0 = D_8012D5A8;
    item = ovl_11_func_8011D98C(*(StructD548 *)D_8012D5A8);
    price = ovl_11_func_800D6014(item);
    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(6, 0);
        break;
    case 1:
        if (func_800226A4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 2:
        first.unk4 = -1;
        first.unk6 = 0xB5;
        ovl_11_func_8011D084(6, 0);
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        switch (ovl_11_func_8011B840(&D_8012D524, 6, first)) {
        case 0:
            break;
        case 1:
            switch (((StructD548 *)D_8012D548)->index) {
            case 0:
                func_8002261C(2, 0x87);
                ovl_11_func_8011D084(1, 2);
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                ovl_11_func_8011D084(20, 0);
                break;
            case 5:
                if (flags & 0x1C0000) {
                    ovl_11_func_8011D084(13, 0);
                } else {
                    func_8002261C(2, 0xAD);
                    ovl_11_func_8011D084(1, 2);
                }
                break;
            }
            break;
        case 3:
            ovl_11_func_8011D084(4, 0);
            break;
        }
        break;
    case 3:
        func_8002261C(2, 0xAF);
        ovl_11_func_8011D084(1, 13);
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        switch (ovl_11_func_8011B840(&D_8012D524, 13, second)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D9DC(item, 1) == 0) {
                func_80022738();
                func_8002261C(2, 0x54);
                ovl_11_func_8011D084(1, 3);
            } else if (ovl_11_func_8011D06C(price) == 1) {
                func_80022738();
                func_8002261C(2, 0xAE);
                ovl_11_func_8011D084(1, 5);
            } else {
                func_80022738();
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(1, 3);
            }
            break;
        case 3:
            D_8012D52C = 4;
            ovl_11_func_8011D084(2, 0);
            break;
        }
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        switch (ovl_11_func_8011BD6C(&D_8012D524, 20, (StructD548 **)&first)) {
        case 0:
            break;
        case 1:
            ovl_11_func_8011D084(4, 0);
            break;
        case 3:
            D_8012D52C = 4;
            func_8002261C(2, 0x91);
            ovl_11_func_8011D084(1, 2);
            break;
        }
        break;
    case 5:
        ovl_11_func_800F2354(-price, 1);
        ovl_11_func_8011DAD4(item, 1, 1);
        ovl_11_func_80111F10();
        ovl_11_func_8011B2B0();
        D_8012D52C = 4;
        func_8002261C(2, 0x91);
        ovl_11_func_8011D084(1, 2);
        break;
    case 4:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
