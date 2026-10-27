#include "common.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_save_counters.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_quantity_input.h"
#include "ovl11_item_slot_commands.h"

void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800F45A4(s16 arg0, s32 arg1);

s32 ovl_11_func_80118CE4(void) {
    s32 result;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(9, 0);
        ovl_11_func_800F45A4(0x84, 0);
        break;
    case 9:
    case 10:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 9, 0x6C)) {
            Ovl11SetInteractionSteps(1, 0);
        }
        break;
    case 1:
        if (((Ovl11FB908State *)D_8006C838)->field_4506 >= 999) {
            RequestMessageWindow(2, 0xD1);
            Ovl11SetInteractionSteps(2, 7);
        } else {
            RequestMessageWindow(2, 0xD0);
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
            Ovl11ResumeInteractionStep();
        }
        break;
    case 4:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        Ovl11SetInteractionSteps(3, 5);
        break;
    case 5:
        gOvl11InteractionDrawMode = 1;
        switch (Ovl11ProcessQuantityInput(999, (u16 *)&gOvl11QuantityPrompt.unk8)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA) == 1) {
                if (gOvl11QuantityPrompt.unk8 + ((Ovl11FB908State *)D_8006C838)->field_4506 >= 1000) {
                    CloseMessageWindow();
                    RequestMessageWindow(2, 0xD2);
                    Ovl11SetInteractionSteps(2, 4);
                } else {
                    CloseMessageWindow();
                    RequestMessageWindow(2, 0xD3);
                    Ovl11SetInteractionSteps(2, 6);
                }
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(2, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x25B);
            Ovl11SetInteractionSteps(2, 4);
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xBA);
            Ovl11SetInteractionSteps(2, 7);
            break;
        }
        break;
    case 6:
        Ovl11AddSaveCounter21(1, (u16)gOvl11QuantityPrompt.unk8);
        Ovl11AdjustSpendableBalance(-(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA), 1);
    case 7:
        ovl_11_func_800F45A4(0x84, 1);
        Ovl11MatchItemSlotsCommand(0x6C, 1, 0, 1);
        Ovl11SetInteractionSteps(8, 0);
        break;
    case 8:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
