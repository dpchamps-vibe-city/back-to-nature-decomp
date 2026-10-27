#include "common.h"
#include "message_window.h"
#include "two_choice_prompt.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_balance_accounting.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_item_slot_commands.h"

void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800F45A4(s16 arg0, s32 arg1);

s32 ovl_11_func_80119820(void) {
    s32 result;
    s16 price;

    if (!(((struct Ovl11CallbackGateState *)D_8006C838)->flags & 0x100)) {
        return 1;
    }
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = Ovl11GetItemPrice(0x21);
    result = 0;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(11, 0);
        break;
    case 11:
    case 12:
        RequestMessageWindow(2, 0xDC);
        SetMessagePortraitEnabled(0);
        Ovl11SetInteractionSteps(3, 4);
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
    case 4:
        gOvl11InteractionDrawMode = 3;
        Ovl11SetInteractionSteps(8, 0);
        Ovl11UpdateWideChoicePrompt(0, -1, 0);
    case 8:
        switch (Ovl11UpdateWideChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            if (Ovl11GetItemStackCapacity(0x21, 1) == 0) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x54);
                Ovl11SetInteractionSteps(2, 7);
            } else if (Ovl11HasSpendableBalance(price) == 1) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0xBB);
                Ovl11SetInteractionSteps(2, 6);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(2, 7);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xBA);
            Ovl11SetInteractionSteps(2, 7);
            break;
        }
        break;
    case 6:
        ovl_11_func_800F45A4(0x21, 0);
        Ovl11AdjustSpendableBalance(-price, 1);
        Ovl11InsertHeldGridOrOverflowItem(0x21, 0, 1, 1);
        ((struct Ovl11CallbackGateState *)D_8006C838)->status |= 0x200000;
        ((struct Ovl11CallbackGateState *)D_8006C838)->flags &= ~0x100;
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    case 7:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
