#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_selection_record_access.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"

s32 ovl_11_func_80119AF0(void) {
    s32 result;
    Ovl11D8012D7B8 params;

    result = 0;
    params.unk4 = 2;
    params.unk6 = 0xB0;
    params.unk8 = 2;
    params.unkA = 0xB1;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(7, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        params.unk4 = 2;
        params.unk6 = 0xB5;
        Ovl11SetInteractionSteps(7, 0);
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 7, params)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(Ovl11GetItemPrice(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) == 1) {
                Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
                CloseMessageWindow();
                RequestMessageWindow(2, 0xB4);
                Ovl11SetInteractionSteps(1, 4);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(1, 2);
            }
            break;
        case 3:
            Ovl11SetInteractionSteps(3, 0);
            break;
        }
        break;
    case 4:
    case 5:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 4, Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) {
            Ovl11SetInteractionSteps(3, 0);
        }
        break;
    case 3:
        result = 1;
        break;
    }
    return result;
}
