#include "common.h"
#include "game_types.h"

void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
void func_80022738(void);
s32 ovl_11_func_80116878(StructD548 *arg0);
void ovl_11_func_801168C0(StructD548 *arg0);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);

s32 ovl_11_func_801165E0(void) {
    Ovl11D8012D7B8 params;
    s32 result;

    result = 0;
    params.unk4 = -1;
    params.unk8 = -1;
    params.unk10 = 7;
    params.unk0 = D_8012D548;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        D_8012D52C = 7;
        ovl_11_func_8011D084(1, 0);
        params.unk4 = -1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        switch (ovl_11_func_8011B840(&D_8012D524, 1, params)) {
        case 0:
            break;
        case 3:
            func_8002261C(1, 0x447);
            ovl_11_func_8011D084(9, 11);
            break;
        case 1:
            if (ovl_11_func_80116878((StructD548 *)D_8012D548) == 1) {
                func_8002261C(1, 0x449);
                ovl_11_func_8011D084(10, 8);
            } else {
                func_8002261C(1, 0x44A);
                ovl_11_func_8011D084(9, 0);
            }
            break;
        case 2:
            break;
        }
        break;
    case 8:
        switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            func_80022738();
            func_8002261C(1, 0x448);
            ovl_11_func_801168C0((StructD548 *)D_8012D548);
            break;
        case 2:
            func_80022738();
            func_8002261C(1, 0x447);
            ovl_11_func_8011D084(9, 0);
            break;
        }
        break;
    case 10:
        if (func_800226A4() == 5) {
            func_80017B18(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 9:
        if (func_800226A4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 11:
        result = 1;
        break;
    case 12:
    case 13:
        if (ovl_11_func_8011B6C4(&D_8012D524, 12, ((StructD548 *)D_8012D548)->data[((StructD548 *)D_8012D548)->index])) {
            ovl_11_func_8011D084(11, 0);
        }
        break;
    }
    return result;
}
