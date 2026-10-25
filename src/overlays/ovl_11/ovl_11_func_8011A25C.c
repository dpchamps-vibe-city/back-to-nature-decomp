#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800E5E98(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9284(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_80111E68(s16 arg0);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s32 ovl_11_func_8011DD90(void);
s32 ovl_11_func_8011DE70(s16 arg0);
u16 ovl_11_func_8011DEBC(s16 arg0, s32 arg1);
s32 ovl_11_func_8011DF04(s16 arg0);

s32 ovl_11_func_8011A25C(void) {
    s32 result;
    s16 item;
    s16 group;
    s32 price;
    s32 index;
    s32 row;
    s16 message;

    result = 0;
    item = ((Ovl11A25CState *)D_8006C838)->field_5252;
    group = ((Ovl11A25CState *)D_8006C838)->field_524C;
    ((Ovl11A25CState *)D_8006C838)->field_522C = 0;
    ((Ovl11A25CState *)D_8006C838)->field_5230 = 0;
    ((Ovl11A25CState *)D_8006C838)->field_5234 |= 0x800000;
    row = (s16)ovl_11_func_8011DF04(item);
    price = ovl_11_func_8011DEBC(row, 1);
    switch (gOvl11InteractionStep) {
    case 0:
        ovl_11_func_8011D084(3, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 15:
        if (((Ovl11A25CState *)D_8006C838)->field_5234 & 0x10000000) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        message = ovl_11_func_8011DD90();
        if (message != 0) {
            RequestMessageWindow(2, D_801282D0[message - 1]);
            ovl_11_func_8011D084(1, 4);
        } else {
            gOvl11InteractionDrawMode = 2;
            SetMessagePortraitEnabled(0);
            RequestMessageWindow(2, ovl_11_func_8011DE70(ovl_11_func_8011DF04(item)) + 0x9A);
            ovl_11_func_8011D084(2, 5);
        }
        break;
    case 5:
        ovl_11_func_8011D084(6, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 6:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            if (ovl_11_func_8011D06C(price) == 1) {
                SetMessagePortraitEnabled(0);
                RequestMessageWindow(2, 0x9E);
                ovl_11_func_8011D084(2, 13);
            } else {
                RequestMessageWindow(2, 0x4F);
                ovl_11_func_8011D084(1, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            ovl_11_func_8011D084(1, 4);
            break;
        }
        break;
    case 13:
        ovl_11_func_8011D084(14, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 14:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            ovl_11_func_8011D084(11, 0);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            ovl_11_func_8011D084(1, 4);
            break;
        }
        break;
    case 11:
        gOvl11InteractionDrawMode = 0;
    case 12:
        ovl_11_func_800F2354(-price, 1);
        index = ovl_11_func_8011DF04(item) + 1;
        row = (group - 1) / 5 * 4;
        row += (group - 1) / 5;
        row += index;
        ovl_11_func_80111E68(row);
        CloseMessageWindow();
        RequestMessageWindow(2, 0x9F);
        ovl_11_func_8011D084(1, 10);
        break;
    case 10:
        ovl_11_func_800E6EE0(item, 1, 0, 1);
        ovl_11_func_800E6EE0(group, 0, 0, 1);
        BeginScreenFadeIncrease(20, 0, 2);
        ovl_11_func_8011D084(9, 0);
    case 9:
        if (IsScreenFadeComplete() == 1) {
            ovl_11_func_800E5E98(32, 0, 0, 0);
            ((Ovl11A25CState *)D_8006C838)->x = 1000;
            ((Ovl11A25CState *)D_8006C838)->z = -2800;
            ((Ovl11A25CState *)D_8006C838)->y = 0;
            ovl_11_func_800E9284(3, 0, 0, 0);
            BeginScreenFadeDecrease(20, 0, 2);
            ovl_11_func_8011D084(8, 0);
        }
        break;
    case 4:
        RequestMessageWindow(2, 0x87);
        ovl_11_func_8011D084(1, 7);
        break;
    case 8:
        if (IsScreenFadeComplete() != 1) {
            break;
        }
        ovl_11_func_800E9B68(4, 0, 0, 0);
        ovl_11_func_8011D084(7, 0);
    case 7:
        ((Ovl11A25CState *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
