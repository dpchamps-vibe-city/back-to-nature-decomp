/* Quantity/name-entry purchase interaction, including inventory insertion
 * and list-selected record release. Its sole cost/quantity helper consumes
 * the same +8/+A prompt fields the handler validates. The name-entry record
 * is private scratch; generic message/selection state remains shared.
 * Original ovl_11:80115308–80115CAC. Contract: ovl11_quantity_purchase.h. */

#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "text_entry.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_record_family_list.h"
#include "ovl11_quantity_purchase.h"
#include "ovl11_selection_record_access.h"
#include "ovl11_save_counters.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_record_family_lookup.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_item_record_allocation.h"
#include "two_choice_prompt.h"
#include "ovl11_price_option_rows.h"
#include "psyq/libapi.h"
#include "text_engine.h"

void Ovl11DebitQuantityCost(void *arg0);

s32 Ovl11UpdateQuantityPurchaseInteraction(void) {
    Ovl11D8012D7B8 params;
    s32 result;
    s32 selected;
    s32 i;
    u16 *dest;
    Ovl11D8012D538 *state;
    struct struct_8006C838_800E8F78 *resident;

    result = 0;
    resident = (struct struct_8006C838_800E8F78 *)D_8006C838;
    params.unk4 = 2;
    params.unk6 = 0x44;
    params.unk8 = 2;
    params.unkA = 0x45;
    params.unkC = 2;
    params.unkE = 0x4E;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    resident->field_522C = 0;
    resident->field_5230 = 0;
    resident->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(2, 0);
        break;
    case 1:
        Ovl11BuildQuantityPriceRows();
        gOvl11InteractionDrawMode = 4;
        params.unk4 = -1;
        Ovl11SetInteractionSteps(2, 0);
        /* Fall through: the newly initialized selection is processed now. */
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 2, params)) {
        case 0:
        case 2:
            break;
        case 3:
            Ovl11SetInteractionSteps(24, 0);
            break;
        case 1:
            switch (((StructD548 *)gOvl11PrimarySelectionRecord)->index) {
            case 0:
                if (((Ovl11FeedGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x4C);
                    Ovl11SetInteractionSteps(22, 1);
                } else {
                    RequestMessageWindow(2, 0x4B);
                    Ovl11SetInteractionSteps(22, 9);
                }
                break;
            case 1:
                if (Ovl11FindFreeB4Record() == 0) {
                    RequestMessageWindow(2, 0x55);
                    Ovl11SetInteractionSteps(22, 21);
                } else if (Ovl11HasSpendableBalance(Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) != 1) {
                    RequestMessageWindow(2, 0x4F);
                    Ovl11SetInteractionSteps(22, 21);
                } else {
                    RequestMessageWindow(2, 0x56);
                    Ovl11SetInteractionSteps(22, 16);
                }
                break;
            case 2:
                if (Ovl11FindFamilyRecordIndex(0, 0, -1) == 0) {
                    RequestMessageWindow(2, 0x59);
                    Ovl11SetInteractionSteps(22, 21);
                } else {
                    RequestMessageWindow(2, 0x58);
                    gOvl11InteractionDrawMode = 0;
                    Ovl11DrawAndProcessRecordFamilyList(0, 0, -1);
                    Ovl11SetInteractionSteps(22, 18);
                }
                break;
            case 3:
                if (Ovl11HasSpendableBalance(Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) == 1) {
                    if (Ovl11GetItemStackCapacity(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 1) == 0) {
                        RequestMessageWindow(2, 0x54);
                        Ovl11SetInteractionSteps(22, 21);
                    } else {
                        Ovl11InsertItemStack(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 1, 1);
                        Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
                        RequestMessageWindow(2, 0x51);
                        Ovl11SetInteractionSteps(22, 21);
                    }
                } else {
                    RequestMessageWindow(2, 0x4F);
                    Ovl11SetInteractionSteps(22, 21);
                }
                break;
            }
            break;
        }
        break;
    case 9:
        SetMessagePortraitEnabled(0);
        SetTextInputEnabled(0);
        RequestMessageWindow(2, 0x4D);
        Ovl11SetInteractionSteps(23, 10);
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        state = &gOvl11QuantityPrompt;
        switch (Ovl11StepQuantityPrompt(&gOvl11InteractionStep, 10, 5, *(Ovl11BAFCParams *)&params, (Ovl11QuantityPromptView *)state)) {
        case 0:
        case 2:
            break;
        case 3:
            Ovl11SetInteractionSteps(22, 21);
            break;
        case 1:
            CloseMessageWindow();
            if (Ovl11HasSpendableBalance(state->unk8 * state->unkA) == 1) {
                if (state->unk8 + ((Ovl11FeedGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x4C);
                    Ovl11SetInteractionSteps(22, 9);
                } else {
                    Ovl11DebitQuantityCost(state);
                    RequestMessageWindow(2, 0x52);
                    Ovl11SetInteractionSteps(22, 21);
                }
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(22, 9);
            }
            break;
        }
        break;
    case 16:
        ResetTextEntry();
        BeginTextEntry(gOvl11QuantityPurchaseNameText);
        gOvl11InteractionDrawMode = 0;
        Ovl11SetInteractionSteps(17, 0);
        break;
    case 17:
        switch (UpdateTextEntry()) {
        case -1:
            RequestMessageWindow(2, 0x4E);
            Ovl11SetInteractionSteps(22, 21);
            break;
        case 1:
            dest = Ovl11AllocateItemRecord(0x109, -1) + 2;
            for (i = 0; i < 9; i++) {
                dest[i] = gOvl11QuantityPurchaseNameRecord.values[i];
            }
            Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
            RequestMessageWindow(2, 0x57);
            Ovl11SetInteractionSteps(22, 21);
            break;
        }
        break;
    case 18:
        SetMessagePortraitEnabled(0);
        SetTextInputEnabled(0);
        RequestMessageWindow(2, 0x5A);
        Ovl11SetInteractionSteps(23, 19);
        break;
    case 19:
        selected = Ovl11DrawAndProcessRecordFamilyList(0, 0, 0);
        if (selected == -2) {
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            Ovl11SetInteractionSteps(22, 21);
        } else if (selected != -1) {
            CloseMessageWindow();
            SetMessagePortraitEnabled(0);
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[selected].flags & 0x40) {
                RequestMessageWindow(2, 0x61);
            } else {
                RequestMessageWindow(2, 0x60);
            }
            Ovl11SetInteractionSteps(23, 20);
        }
        break;
    case 20:
        selected = Ovl11DrawAndProcessRecordFamilyList(0, 0, 1);
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5E);
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[selected].flags & 0x40) {
                Ovl11AdjustSpendableBalance(2000, 1);
            } else {
                Ovl11AdjustSpendableBalance(500, 1);
            }
            Ovl11ClearSmallRecord((s32)((u8 *)&gSmallTaggedRecordBase + selected * 0xB4));
            Ovl11SetInteractionSteps(22, 21);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            Ovl11SetInteractionSteps(22, 21);
            break;
        }
        break;
    case 21:
        RequestMessageWindow(2, 0x53);
        Ovl11SetInteractionSteps(22, 1);
        break;
    case 22:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 23:
        if (gOvl11InteractionResumeStep == 20) {
            Ovl11DrawAndProcessRecordFamilyList(0, 0, 1);
        }
        if (GetMessageWindowStatus() == 5) {
            SetTextInputEnabled(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 24:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}

/* The caller supplies gOvl11QuantityPrompt: signed quantity/cost at +8/+A. */
void Ovl11DebitQuantityCost(void *record) {
    Ovl11AdjustSpendableBalance(-(((Ovl11D8012D538 *)record)->unkA *
                          ((Ovl11D8012D538 *)record)->unk8), 1);
    Ovl11AddSaveCounter20(2, ((Ovl11D8012D538 *)record)->unk8);
}
