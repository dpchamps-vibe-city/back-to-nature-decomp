#include "common.h"
#include "message_window.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800CB850(s32 arg0);
void ovl_11_func_800E2904(s32 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s16 ovl_11_func_80115F38(void);
s32 ovl_11_func_80115F80(void);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);

s32 ovl_11_func_80115CAC(void) {
    s32 result;
    s16 id;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    id = ((struct Ovl11E8C14State *)D_8006C838)->value;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(1, 0);
        break;
    case 1:
        if (id == 0x10A) {
            RequestMessageWindow(2, 0x5C);
            ovl_11_func_8011D084(5, 7);
        } else if (id == 0x109) {
            SetMessagePortraitEnabled(0);
            if (ovl_11_func_80115F80() == 1) {
                RequestMessageWindow(2, 0x61);
            } else {
                RequestMessageWindow(2, 0x60);
            }
            ovl_11_func_8011D084(6, 2);
        }
        break;
    case 2:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 200, 128)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x51);
            if (ovl_11_func_80115F80() == 1) {
                ovl_11_func_800F2354(2000, 1);
            } else {
                ovl_11_func_800F2354(500, 1);
            }
            ovl_11_func_800E2904((s32)((u8 *)&D_800742EC + ovl_11_func_80115F38() * 0xB4));
            ovl_11_func_8011D084(5, 3);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            ovl_11_func_8011D084(5, 7);
            break;
        }
        break;
    case 3:
        ovl_11_func_800CB850(0);
        ovl_11_func_8011D084(4, 0);
        break;
    case 4:
        if (!(((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x10000)) {
            ovl_11_func_8011D084(7, 0);
        }
        break;
    case 5:
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 6:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 7:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
