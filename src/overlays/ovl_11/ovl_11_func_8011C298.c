#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "message_window.h"

/* Original overlay labels for the resident setters and state getter. */
void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800C4FDC(Ovl11C7010Object *arg0);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
void ovl_11_func_800F45A4(s16 arg0, s32 arg1);
void ovl_11_func_80111F10(void);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s32 ovl_11_func_8011D35C(s32 arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_8011C298(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    VECTOR position;
    s32 i;
    s32 result;
    s16 price;
    s16 message;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = ovl_11_func_800D6014(arg0);
    message = 0xBA;
    if (arg3 == 0) {
        message = 0xD8;
    }
    result = 0;
    switch (gOvl11InteractionStep) {
    case 0:
        if (arg3 != 0) {
            ovl_11_func_800F45A4(arg0, 0);
        }
        ovl_11_func_8011D084(12, 0);
        break;
    case 12:
    case 13:
        if (ovl_11_func_8011B6C4(&gOvl11InteractionStep, 12, arg0)) {
            RequestMessageWindow(arg1, arg2);
            ovl_11_func_8011D084(3, 11);
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 3:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 11:
        if (((GfxObj *)gPad0Input)->field_8 & 0x60) {
            SetMessagePortraitEnabled(0);
            ovl_11_func_8011D084(4, 4);
        }
        break;
    case 4:
        gOvl11InteractionDrawMode = 3;
        ovl_11_func_8011D084(8, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 8:
        switch (ovl_11_func_8011D35C(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(price) == 1) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0xBB);
                ovl_11_func_8011D084(2, 6);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                ovl_11_func_8011D084(2, 7);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, message);
            ovl_11_func_8011D084(2, 7);
            break;
        }
        break;
    case 6:
        ovl_11_func_800F2354(-price, 1);
        ovl_11_func_8011D084(9, 0);
        if (arg3 == 2) {
            ovl_11_func_800F45A4(arg0, 1);
            ovl_11_func_800E6EE0(arg0, 1, 0, 1);
            ovl_11_func_8011D084(10, 0);
        } else if (arg3 == 1) {
            ovl_11_func_800C4FDC((Ovl11C7010Object *)((Ovl11PurchaseState *)D_8006C838)->object);
            ((Ovl11PurchaseState *)D_8006C838)->level++;
            if (((Ovl11PurchaseState *)D_8006C838)->level >= 3) {
                ((Ovl11PurchaseState *)D_8006C838)->level = 2;
            }
            ((Ovl11PurchaseState *)D_8006C838)->action = 10;
        }
        break;
    case 7:
        if (arg3 != 0) {
            ovl_11_func_800F45A4(arg0, 1);
        }
        ovl_11_func_800E6EE0(arg0, 1, 0, 1);
        ovl_11_func_8011D084(9, 0);
        break;
    case 9:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        if (arg3 == 1) {
            ovl_11_func_80111F10();
        }
        break;
    case 10:
        position.vx = -5000;
        position.vy = 0;
        position.vz = 2300;
        ((Ovl11PurchaseState *)D_8006C838)->count = 0;
        ((Ovl11PurchaseState *)D_8006C838)->total = 0;
        ((Ovl11PurchaseState *)D_8006C838)->status |= 0x800;
        for (i = 0; i < 25; i++) {
            ((Ovl11PurchaseState *)D_8006C838)->entries[i] = 0;
        }
        ovl_11_func_800F43CC(100, position, 1, 0);
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
