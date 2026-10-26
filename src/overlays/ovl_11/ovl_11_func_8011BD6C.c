#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_progress_status.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800E5E98(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9284(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);
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
                    SetMessagePortraitEnabled(0);
                    RequestMessageWindow(messages[0][0], messages[0][1]);
                    Ovl11SetInteractionStepsOffset(2, 4, arg1);
                } else {
                    RequestMessageWindow(messages[1][0], messages[1][1]);
                    Ovl11SetInteractionStepsOffset(1, 3, arg1);
                }
            } else {
                RequestMessageWindow(messages[2][0], messages[2][1]);
                Ovl11SetInteractionStepsOffset(1, 3, arg1);
            }
            break;
        case 1:
            if (GetMessageWindowStatus() == 2) {
                Ovl11ResumeInteractionStep();
            }
            break;
        case 2:
            if (GetMessageWindowStatus() == 5) {
                SetVal8005E2BC(0);
                Ovl11ResumeInteractionStep();
            }
            break;
        case 13:
            if (gPrimaryObjectFlags & 0x10000000) {
                Ovl11ResumeInteractionStep();
            }
            break;
        case 4:
            Ovl11SetInteractionStepsOffset(5, 0, arg1);
            ovl_11_func_80118B84(0, -1, 0);
        case 5:
            switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
            case 0:
                break;
            case 1:
                CloseMessageWindow();
                SetMessagePortraitEnabled(0);
                RequestMessageWindow(messages[3][0], messages[3][1]);
                Ovl11SetInteractionStepsOffset(2, 6, arg1);
                break;
            case 2:
                CloseMessageWindow();
                RequestMessageWindow(2, 0x93);
                Ovl11SetInteractionStepsOffset(1, 3, arg1);
                break;
            }
            break;
        case 6:
            Ovl11SetInteractionStepsOffset(7, 0, arg1);
            ovl_11_func_80118B84(0, -1, 0);
        case 7:
            switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
            case 0:
                break;
            case 1:
                CloseMessageWindow();
                Ovl11SetInteractionStepsOffset(8, 0, arg1);
                break;
            case 2:
                CloseMessageWindow();
                RequestMessageWindow(2, 0x93);
                Ovl11SetInteractionStepsOffset(1, 3, arg1);
                break;
            }
            break;
        case 8:
            gOvl11InteractionDrawMode = 0;
            ovl_11_func_800E6EE0(mode, 1, 1, 1);
            ovl_11_func_800E6F34(mode, 0, 0, 0);
            Ovl11SetInteractionStepsOffset(13, 14, arg1);
            break;
        case 14:
            ovl_11_func_800F2354(-ovl_11_func_800D6014(item), 1);
            Ovl11RequestProgressAction(item);
            CloseMessageWindow();
            RequestMessageWindow(2, 0x9F);
            Ovl11SetInteractionStepsOffset(1, 12, arg1);
            break;
        case 12:
            ovl_11_func_800E6EE0(mode, 1, 0, 1);
            BeginScreenFadeIncrease(20, 0, 2);
            Ovl11SetInteractionStepsOffset(10, 0, arg1);
        case 10:
            if (IsScreenFadeComplete() == 1) {
                ovl_11_func_800E5E98(32, 0, 0, 0);
                ((Ovl11A25CState *)D_8006C838)->x = 1000;
                ((Ovl11A25CState *)D_8006C838)->z = -2800;
                ((Ovl11A25CState *)D_8006C838)->y = 0;
                ovl_11_func_800E9284(3, 0, 0, 0);
                BeginScreenFadeDecrease(20, 0, 2);
                Ovl11SetInteractionStepsOffset(11, 0, arg1);
            }
            break;
        case 11:
            if (IsScreenFadeComplete() != 1) {
                break;
            }
            Ovl11SetInteractionStepsOffset(9, 0, arg1);
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
