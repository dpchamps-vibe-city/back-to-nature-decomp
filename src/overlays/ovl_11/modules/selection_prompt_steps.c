/*
 * Descriptor-driven selection, quantity and purchase message steps.
 * All three share the global interaction-step/resume and selection protocol.
 * See ovl11_selection_prompt_steps.h for results, ABI and side effects.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_selection_record_access.h"
#include "screen_fade.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_progress_status.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_quantity_input.h"
#include "ovl11_quantity_prompt_setup.h"
#include "two_choice_prompt.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_object_event_slot_reset_command.h"
#include "ovl11_object_state_selection_command.h"

void SetVal8005E2BC(s32 value);
s32 ovl_11_func_800E5E98(s16 a, s16 b, s32 c, s32 d);

s32 Ovl11StepSelectionPrompt(s32 *step, s32 baseStep, Ovl11D8012D7B8 descriptor) {
    s32 result;
    StructD548 *record;
    s16 startKind;
    s16 startId;
    s16 cancelKind;
    s16 cancelId;
    s32 drawMode;

    if ((u32)(*step - baseStep) >= 6U) {
        return -1;
    }
    result = 0;
    record = (StructD548 *)descriptor.unk0;
    startKind = descriptor.unk4;
    startId = descriptor.unk6;
    cancelKind = descriptor.unk8;
    cancelId = descriptor.unkA;
    drawMode = descriptor.unk10;
    switch (*step - baseStep) {
    case 0:
        if (startKind != -1) {
            RequestMessageWindow(startKind, startId);
            Ovl11SetInteractionStepsOffset(1, 3, baseStep);
        } else {
            Ovl11SetInteractionStepsOffset(3, 0, baseStep);
        }
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        if (gOvl11InteractionDrawMode == 7) {
            gOvl11InteractionDrawMode = drawMode;
            if (RequestMessageWindow(2, 0x5F) == 0) {
                Ovl11SetInteractionStepsOffset(2, 4, baseStep);
            }
        } else {
            gOvl11InteractionDrawMode = drawMode;
            if (RequestMessageWindow(2, 0x46) == 0) {
                Ovl11SetInteractionStepsOffset(2, 4, baseStep);
            }
        }
        break;
    case 4:
        switch (Ovl11ProcessSelectionRecordInput((Ovl11D548Arg *)record)) {
        case 0:
            break;
        case 1:
            result = 1;
            CloseMessageWindow();
            break;
        case 2:
            if (Ovl11GetSelectionMessageId(*record) != -1) {
                CloseMessageWindow();
                RequestMessageWindow(2, Ovl11GetSelectionMessageId(*record));
                Ovl11SetInteractionStepsOffset(1, 3, baseStep);
            }
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(cancelKind, cancelId);
            Ovl11SetInteractionStepsOffset(1, 5, baseStep);
            break;
        }
        break;
    case 5:
        result = 3;
        break;
    }
    return result;
}

s32 Ovl11StepQuantityPrompt(s32 *step, s32 baseStep, s32 drawMode,
                          Ovl11BAFCParams descriptor, Ovl11QuantityPromptView *prompt) {
    s32 result;
    StructD548 *record;

    if ((u32)(*step - baseStep) >= 6U) {
        return -1;
    }
    record = descriptor.record;
    result = 0;
    switch (*step - baseStep) {
    case 0:
        Ovl11SetInteractionStepsOffset(3, 0, baseStep);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        Ovl11SetupQuantityPrompt(prompt, 0, 0,
            (s16)Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*record)), 0);
        gOvl11InteractionDrawMode = drawMode;
        Ovl11SetInteractionStepsOffset(2, 4, baseStep);
        break;
    case 4:
        switch (Ovl11ProcessQuantityInput(0x63, (u16 *)&prompt->quantity)) {
        case 0:
            break;
        case 1:
            result = 1;
            CloseMessageWindow();
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, Ovl11GetSelectionMessageId(*record));
            Ovl11SetInteractionStepsOffset(1, 3, baseStep);
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(descriptor.field_C, descriptor.field_E);
            Ovl11SetInteractionStepsOffset(1, 5, baseStep);
            break;
        }
        break;
    case 5:
        result = 3;
        break;
    }
    return result;
}

s32 Ovl11StepSelectedItemPurchase(s32 *step, s32 baseStep, StructD548 **record) {
    s16 messages[4][2];
    s32 state;
    s32 valid;
    s32 result;
    s16 item;
    s16 mode;

    state = *step - baseStep;
    if ((u32)state >= 15U) {
        return -1;
    }
    item = Ovl11GetSelectionEntryId(**record);
    valid = (*record)->index != 1;
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
            if (Ovl11MatchItemSlotsCommand(mode, 1, 1, 0) == 1) {
                if (Ovl11HasSpendableBalance(Ovl11GetItemPrice(item)) == 1) {
                    SetMessagePortraitEnabled(0);
                    RequestMessageWindow(messages[0][0], messages[0][1]);
                    Ovl11SetInteractionStepsOffset(2, 4, baseStep);
                } else {
                    RequestMessageWindow(messages[1][0], messages[1][1]);
                    Ovl11SetInteractionStepsOffset(1, 3, baseStep);
                }
            } else {
                RequestMessageWindow(messages[2][0], messages[2][1]);
                Ovl11SetInteractionStepsOffset(1, 3, baseStep);
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
            Ovl11SetInteractionStepsOffset(5, 0, baseStep);
            Ovl11UpdateCompactChoicePrompt(0, -1, 0);
            /* Initialize and poll the first confirmation in the same call. */
        case 5:
            switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
            case 0:
                break;
            case 1:
                CloseMessageWindow();
                SetMessagePortraitEnabled(0);
                RequestMessageWindow(messages[3][0], messages[3][1]);
                Ovl11SetInteractionStepsOffset(2, 6, baseStep);
                break;
            case 2:
                CloseMessageWindow();
                RequestMessageWindow(2, 0x93);
                Ovl11SetInteractionStepsOffset(1, 3, baseStep);
                break;
            }
            break;
        case 6:
            Ovl11SetInteractionStepsOffset(7, 0, baseStep);
            Ovl11UpdateCompactChoicePrompt(0, -1, 0);
            /* Likewise for the second confirmation. */
        case 7:
            switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xE0, 0x80)) {
            case 0:
                break;
            case 1:
                CloseMessageWindow();
                Ovl11SetInteractionStepsOffset(8, 0, baseStep);
                break;
            case 2:
                CloseMessageWindow();
                RequestMessageWindow(2, 0x93);
                Ovl11SetInteractionStepsOffset(1, 3, baseStep);
                break;
            }
            break;
        case 8:
            gOvl11InteractionDrawMode = 0;
            Ovl11MatchItemSlotsCommand(mode, 1, 1, 1);
            Ovl11InsertSecondaryItemCommand(mode, 0, 0, 0);
            Ovl11SetInteractionStepsOffset(13, 14, baseStep);
            break;
        case 14:
            Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice(item), 1);
            Ovl11RequestProgressAction(item);
            CloseMessageWindow();
            RequestMessageWindow(2, 0x9F);
            Ovl11SetInteractionStepsOffset(1, 12, baseStep);
            break;
        case 12:
            Ovl11MatchItemSlotsCommand(mode, 1, 0, 1);
            BeginScreenFadeIncrease(20, 0, 2);
            Ovl11SetInteractionStepsOffset(10, 0, baseStep);
        case 10:
            if (IsScreenFadeComplete() == 1) {
                ovl_11_func_800E5E98(32, 0, 0, 0);
                ((Ovl11A25CState *)D_8006C838)->x = 1000;
                ((Ovl11A25CState *)D_8006C838)->z = -2800;
                ((Ovl11A25CState *)D_8006C838)->y = 0;
                Ovl11SelectObjectStateOrDirectionCommand(3, 0, 0, 0);
                BeginScreenFadeDecrease(20, 0, 2);
                Ovl11SetInteractionStepsOffset(11, 0, baseStep);
            }
            break;
        case 11:
            if (IsScreenFadeComplete() != 1) {
                break;
            }
            Ovl11SetInteractionStepsOffset(9, 0, baseStep);
        case 9:
            Ovl11ResetObjectEventSlotCommand(4, 0, 0, 0);
            result = 1;
            break;
        case 3:
            result = 3;
            break;
        }
    }
    return result;
}
