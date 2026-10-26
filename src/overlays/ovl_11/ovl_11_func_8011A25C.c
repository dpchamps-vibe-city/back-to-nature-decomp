#include "common.h"
#include "screen_fade.h"
#include "message_window.h"
#include "ovl11_tier_upgrade_rules.h"
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
#include "ovl11_balance_accounting.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800E5E98(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9284(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);

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
    row = (s16)Ovl11GetUpgradeTierForItem(item);
    price = Ovl11GetUpgradeValue(row, 1);
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(3, 0);
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
    case 15:
        if (((Ovl11A25CState *)D_8006C838)->field_5234 & 0x10000000) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        message = Ovl11CheckTierUpgrade();
        if (message != 0) {
            RequestMessageWindow(2, D_801282D0[message - 1]);
            Ovl11SetInteractionSteps(1, 4);
        } else {
            gOvl11InteractionDrawMode = 2;
            SetMessagePortraitEnabled(0);
            RequestMessageWindow(2, Ovl11GetItemTier(Ovl11GetUpgradeTierForItem(item)) + 0x9A);
            Ovl11SetInteractionSteps(2, 5);
        }
        break;
    case 5:
        Ovl11SetInteractionSteps(6, 0);
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
                Ovl11SetInteractionSteps(2, 13);
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(1, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            Ovl11SetInteractionSteps(1, 4);
            break;
        }
        break;
    case 13:
        Ovl11SetInteractionSteps(14, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 14:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            Ovl11SetInteractionSteps(11, 0);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x93);
            Ovl11SetInteractionSteps(1, 4);
            break;
        }
        break;
    case 11:
        gOvl11InteractionDrawMode = 0;
    case 12:
        Ovl11AdjustSpendableBalance(-price, 1);
        index = Ovl11GetUpgradeTierForItem(item) + 1;
        row = (group - 1) / 5 * 4;
        row += (group - 1) / 5;
        row += index;
        Ovl11RequestProgressAction(row);
        CloseMessageWindow();
        RequestMessageWindow(2, 0x9F);
        Ovl11SetInteractionSteps(1, 10);
        break;
    case 10:
        ovl_11_func_800E6EE0(item, 1, 0, 1);
        ovl_11_func_800E6EE0(group, 0, 0, 1);
        BeginScreenFadeIncrease(20, 0, 2);
        Ovl11SetInteractionSteps(9, 0);
    case 9:
        if (IsScreenFadeComplete() == 1) {
            ovl_11_func_800E5E98(32, 0, 0, 0);
            ((Ovl11A25CState *)D_8006C838)->x = 1000;
            ((Ovl11A25CState *)D_8006C838)->z = -2800;
            ((Ovl11A25CState *)D_8006C838)->y = 0;
            ovl_11_func_800E9284(3, 0, 0, 0);
            BeginScreenFadeDecrease(20, 0, 2);
            Ovl11SetInteractionSteps(8, 0);
        }
        break;
    case 4:
        RequestMessageWindow(2, 0x87);
        Ovl11SetInteractionSteps(1, 7);
        break;
    case 8:
        if (IsScreenFadeComplete() != 1) {
            break;
        }
        ovl_11_func_800E9B68(4, 0, 0, 0);
        Ovl11SetInteractionSteps(7, 0);
    case 7:
        ((Ovl11A25CState *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
