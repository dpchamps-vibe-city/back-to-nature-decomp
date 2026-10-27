#include "common.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_balance_accounting.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011D06C(s32 arg0);

s32 ovl_11_func_80119008(void) {
    s32 result;
    s16 price;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = Ovl11GetItemPrice(0xA4);
    result = 0;
    switch (gOvl11InteractionStep) {
    case 0:
        D_8012D51C = ((struct Ovl11E8C14State *)D_8006C838)->value;
        Ovl11SetInteractionSteps(1, 0);
    case 1:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x10000000) {
            if (D_8012D51C == 0xA4) {
                RequestMessageWindow(2, 0xBC);
                Ovl11SetInteractionSteps(2, 4);
            } else if (Ovl11ClassifyHeldItemType((u16)D_8012D51C) == 1) {
                RequestMessageWindow(2, 0xB6);
                SetMessagePortraitEnabled(0);
                Ovl11SetInteractionSteps(3, 5);
            } else {
                RequestMessageWindow(2, 0xB9);
                Ovl11SetInteractionSteps(2, 4);
            }
        } else {
            RequestMessageWindow(2, 0xB8);
            Ovl11SetInteractionSteps(2, 4);
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 5:
        gOvl11InteractionDrawMode = 2;
        Ovl11SetInteractionSteps(6, 0);
        ovl_11_func_80118B84(0, -1, 0);
    case 6:
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(price) == 1) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0xBB);
                Ovl11SetInteractionSteps(2, 7);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(2, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xB7);
            Ovl11SetInteractionSteps(2, 4);
            break;
        }
        break;
    case 7:
        gOvl11InteractionDrawMode = 0;
        Ovl11AdjustSpendableBalance(-price, 1);
        ovl_11_func_800E6EE0(D_8012D51C, 1, 0, 1);
        Ovl11SetInteractionSteps(8, 4);
        break;
    case 8:
        if (((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 & 0x4000000) {
            Ovl11SetInteractionSteps(10, 0);
        }
        break;
    case 10:
    case 11:
        if (Ovl11StepPrimaryObjectA4Request(&gOvl11InteractionStep, 10, D_8012D51C)) {
            Ovl11SetInteractionSteps(4, 0);
        }
        break;
    case 4:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
