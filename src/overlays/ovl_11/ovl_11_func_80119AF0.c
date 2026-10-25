#include "common.h"
#include "game_types.h"
#include "message_window.h"

s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 arg0);

s32 ovl_11_func_80119AF0(void) {
    s32 result;
    Ovl11D8012D7B8 params;

    result = 0;
    params.unk4 = 2;
    params.unk6 = 0xB0;
    params.unk8 = 2;
    params.unkA = 0xB1;
    params.unk10 = 4;
    params.unk0 = D_8012D548;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(7, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 2:
        params.unk4 = 2;
        params.unk6 = 0xB5;
        ovl_11_func_8011D084(7, 0);
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        switch (ovl_11_func_8011B840(&D_8012D524, 7, params)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(ovl_11_func_800D6014(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548))) == 1) {
                ovl_11_func_800F2354(-ovl_11_func_800D6014(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548)), 1);
                CloseMessageWindow();
                RequestMessageWindow(2, 0xB4);
                ovl_11_func_8011D084(1, 4);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                ovl_11_func_8011D084(1, 2);
            }
            break;
        case 3:
            ovl_11_func_8011D084(3, 0);
            break;
        }
        break;
    case 4:
    case 5:
        if (ovl_11_func_8011B6C4(&D_8012D524, 4, ovl_11_func_8011D98C(*(StructD548 *)D_8012D548))) {
            ovl_11_func_8011D084(3, 0);
        }
        break;
    case 3:
        result = 1;
        break;
    }
    return result;
}
