#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_selection_record_access.h"

s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_8011D06C(s32 arg0);

s32 ovl_11_func_8011C6CC(s16 *arg0, s16 *arg1, s16 *arg2, Ovl11D8012D7B8 arg3) {
    s32 result;
    s16 startKind;
    s16 startId;
    s16 nextKind;
    s16 nextId;
    s16 cancelKind;
    s16 cancelId;
    s32 message;
    Ovl11D8012D538 *recordState;
    Ovl11D8012D538 *secondState;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    startKind = arg0[0];
    startId = arg0[1];
    nextKind = arg1[0];
    nextId = arg1[1];
    cancelKind = arg2[0];
    cancelId = arg2[1];
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(4, 0);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        switch (ovl_11_func_8011B840(&gOvl11InteractionStep, 4, arg3)) {
        case 0:
            break;
        case 1:
            if (Ovl11GetItemStackCapacity(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 0) == 0) {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x54);
                Ovl11SetInteractionSteps(1, 2);
            } else {
                gOvl11QuantityPromptCount = 1;
                RequestMessageWindow(startKind, startId);
                Ovl11SetInteractionSteps(1, 11);
            }
            break;
        case 3:
            Ovl11SetInteractionSteps(2, 0);
            break;
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        recordState = &gOvl11QuantityPrompt;
        switch (ovl_11_func_8011BAFC(&gOvl11InteractionStep, 11, 5, *(Ovl11BAFCParams *)&arg3, (ovl_11_DD48_arg0 *)recordState)) {
        case 0:
            break;
        case 1:
            if (ovl_11_func_8011D06C(recordState->unk8 * recordState->unkA) == 1) {
                if (Ovl11GetItemStackCapacity(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 0) < recordState->unk8) {
                    RequestMessageWindow(2, 0xC1);
                    Ovl11SetInteractionSteps(1, 11);
                } else {
                    secondState = &gOvl11QuantityPrompt;
                    if (Ovl11GetItemStackCapacity(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 1) < secondState->unk8) {
                        message = 0x207;
                        if (gOvl11InteractionId == 14) {
                            message = 0x261;
                        }
                        RequestMessageWindow(3, message);
                        Ovl11SetInteractionSteps(1, 3);
                    } else {
                        gOvl11InteractionDrawMode = 4;
                        RequestMessageWindow(nextKind, nextId);
                        Ovl11SetInteractionSteps(1, 7);
                    }
                    ovl_11_func_800F2354(-(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA), 1);
                    Ovl11InsertItemStack(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), gOvl11QuantityPrompt.unk8, 0);
                }
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(1, 11);
            }
            break;
        case 3:
            gOvl11InteractionDrawMode = 4;
            RequestMessageWindow(cancelKind, cancelId);
            Ovl11SetInteractionSteps(1, 7);
            break;
        }
        break;
    case 3:
        RequestMessageWindow(nextKind, nextId);
        Ovl11SetInteractionSteps(1, 7);
        break;
    case 2:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
