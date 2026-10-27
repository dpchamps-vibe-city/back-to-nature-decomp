/*
 * purchase_transactions — reconstructed translation unit.
 *
 * Non-reentrant single-item and descriptor-driven quantity purchase state
 * machines, sharing message retries, balance checks and transaction input
 * suppression.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_purchase_transactions.h"
#include "game_types.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_selection_record_access.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "message_window.h"
#include "two_choice_prompt.h"
#include "ovl11_interaction_steps.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_progress_status.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_primary_object_grid_actions.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_dispatch_record_enable.h"

/* Original overlay labels for the resident setters and state getter. */
void SetVal8005E2BC(s32 arg0);

s32 Ovl11StepSingleItemPurchase(s16 catalogId, s16 messageBank, s16 promptId, s16 mode) {
    VECTOR position;
    s32 i;
    s32 result;
    s16 price;
    s16 message;

    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    price = Ovl11GetItemPrice(catalogId);
    message = 0xBA;
    if (mode == 0) {
        message = 0xD8;
    }
    result = 0;
    switch (gOvl11InteractionStep) {
    case 0:
        if (mode != 0) {
            Ovl11SetDispatchRecordEnabled(catalogId, 0);
        }
        Ovl11SetInteractionSteps(12, 0);
        break;
    case 12:
    case 13:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 12, catalogId)) {
            RequestMessageWindow(messageBank, promptId);
            Ovl11SetInteractionSteps(3, 11);
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
    case 11:
        if (((GfxObj *)gPad0Input)->field_8 & 0x60) {
            SetMessagePortraitEnabled(0);
            Ovl11SetInteractionSteps(4, 4);
        }
        break;
    case 4:
        gOvl11InteractionDrawMode = 3;
        Ovl11SetInteractionSteps(8, 0);
        Ovl11UpdateCompactChoicePrompt(0, -1, 0);
        /* Initialize and process the confirmation in the same update. */
    case 8:
        switch (Ovl11UpdateWideChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 200, 0x80)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(price) == 1) {
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
            RequestMessageWindow(2, message);
            Ovl11SetInteractionSteps(2, 7);
            break;
        }
        break;
    case 6:
        Ovl11AdjustSpendableBalance(-price, 1);
        Ovl11SetInteractionSteps(9, 0);
        if (mode == 2) {
            Ovl11SetDispatchRecordEnabled(catalogId, 1);
            Ovl11MatchItemSlotsCommand(catalogId, 1, 0, 1);
            Ovl11SetInteractionSteps(10, 0);
        } else if (mode == 1) {
            Ovl11SelectHeldPairChangeAnimation((Ovl11C7010Object *)((Ovl11PurchaseState *)D_8006C838)->object);
            ((Ovl11PurchaseState *)D_8006C838)->level++;
            if (((Ovl11PurchaseState *)D_8006C838)->level >= 3) {
                ((Ovl11PurchaseState *)D_8006C838)->level = 2;
            }
            ((Ovl11PurchaseState *)D_8006C838)->action = 10;
        }
        break;
    case 7:
        if (mode != 0) {
            Ovl11SetDispatchRecordEnabled(catalogId, 1);
        }
        Ovl11MatchItemSlotsCommand(catalogId, 1, 0, 1);
        Ovl11SetInteractionSteps(9, 0);
        break;
    case 9:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        if (mode == 1) {
            Ovl11RefreshProgressStatus();
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
        Ovl11SetSpawnRecord(100, position, 1, 0);
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}

s32 Ovl11StepQuantityPurchase(s16 *startMessage, s16 *nextMessage, s16 *cancelMessage, Ovl11D8012D7B8 selection) {
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
    startKind = startMessage[0];
    startId = startMessage[1];
    nextKind = nextMessage[0];
    nextId = nextMessage[1];
    cancelKind = cancelMessage[0];
    cancelId = cancelMessage[1];
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
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 4, selection)) {
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
        switch (Ovl11StepQuantityPrompt(&gOvl11InteractionStep, 11, 5, *(Ovl11BAFCParams *)&selection, (Ovl11QuantityPromptView *)recordState)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(recordState->unk8 * recordState->unkA) == 1) {
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
                    /* The capacity-warning path still commits the purchase. */
                    Ovl11AdjustSpendableBalance(-(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA), 1);
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
