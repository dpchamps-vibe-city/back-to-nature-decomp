#include "common.h"
#include "game_types.h"
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
#include "ovl11_item_stack_insert.h"
#include "ovl11_selection_record_access.h"
#include "ovl11_selection_prompt_steps.h"

s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_8011B2B0(void);
s32 ovl_11_func_8011D06C(s32 arg0);

s32 ovl_11_func_80119DF8(void) {
    Ovl11D8012D7B8 first;
    Ovl11D8012D7B8 second;
    s32 result;
    s32 flags;
    s16 item;
    s16 price;

    flags = ((struct Ovl11CallbackGateState *)D_8006C838)->flags;
    first.unk4 = -1;
    first.unk6 = 0x85;
    first.unk8 = 2;
    first.unkA = 0x86;
    first.unk10 = 4;
    first.unk0 = gOvl11PrimarySelectionRecord;
    second.unk4 = -1;
    second.unk6 = 0;
    second.unk8 = 2;
    second.unkA = 0x91;
    second.unk10 = 6;
    second.unk0 = gOvl11SecondarySelectionRecord;
    item = Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11SecondarySelectionRecord);
    price = ovl_11_func_800D6014(item);
    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(6, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        first.unk4 = -1;
        first.unk6 = 0xB5;
        Ovl11SetInteractionSteps(6, 0);
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 6, first)) {
        case 0:
            break;
        case 1:
            switch (((StructD548 *)gOvl11PrimarySelectionRecord)->index) {
            case 0:
                RequestMessageWindow(2, 0x87);
                Ovl11SetInteractionSteps(1, 2);
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                Ovl11SetInteractionSteps(20, 0);
                break;
            case 5:
                if (flags & 0x1C0000) {
                    Ovl11SetInteractionSteps(13, 0);
                } else {
                    RequestMessageWindow(2, 0xAD);
                    Ovl11SetInteractionSteps(1, 2);
                }
                break;
            }
            break;
        case 3:
            Ovl11SetInteractionSteps(4, 0);
            break;
        }
        break;
    case 3:
        RequestMessageWindow(2, 0xAF);
        Ovl11SetInteractionSteps(1, 13);
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 13, second)) {
        case 0:
            break;
        case 1:
            if (Ovl11GetItemStackCapacity(item, 1) == 0) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x54);
                Ovl11SetInteractionSteps(1, 3);
            } else if (ovl_11_func_8011D06C(price) == 1) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0xAE);
                Ovl11SetInteractionSteps(1, 5);
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(1, 3);
            }
            break;
        case 3:
            gOvl11InteractionDrawMode = 4;
            Ovl11SetInteractionSteps(2, 0);
            break;
        }
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
        switch (Ovl11StepSelectedItemPurchase(&gOvl11InteractionStep, 20, (StructD548 **)&first)) {
        case 0:
            break;
        case 1:
            Ovl11SetInteractionSteps(4, 0);
            break;
        case 3:
            gOvl11InteractionDrawMode = 4;
            RequestMessageWindow(2, 0x91);
            Ovl11SetInteractionSteps(1, 2);
            break;
        }
        break;
    case 5:
        ovl_11_func_800F2354(-price, 1);
        Ovl11InsertItemStack(item, 1, 1);
        Ovl11RefreshProgressStatus();
        ovl_11_func_8011B2B0();
        gOvl11InteractionDrawMode = 4;
        RequestMessageWindow(2, 0x91);
        Ovl11SetInteractionSteps(1, 2);
        break;
    case 4:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
