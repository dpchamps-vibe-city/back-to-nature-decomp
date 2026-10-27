#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "text_entry.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_record_family_list.h"
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
#include "ovl11_record_family_lookup.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_item_record_allocation.h"
#include "two_choice_prompt.h"

void SetVal8005E2BC(s32 arg0);
void ovl_11_func_800DF0F8(s32 arg0);
s32 ovl_11_func_80113CD0(void);
void ovl_11_func_801152BC(void *arg0);

s32 ovl_11_func_80114604(void) {
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
    params.unk6 = 0x62;
    params.unk8 = 2;
    params.unkA = 0x6B;
    params.unkC = 2;
    params.unkE = 0x6E;
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
        ovl_11_func_80113CD0();
        gOvl11InteractionDrawMode = 4;
        params.unk4 = -1;
        Ovl11SetInteractionSteps(2, 0);
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
            Ovl11SetInteractionSteps(27, 0);
            break;
        case 1:
            switch (((StructD548 *)gOvl11PrimarySelectionRecord)->index) {
            case 0:
                if (((Ovl11SaleGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x6D);
                    Ovl11SetInteractionSteps(25, 1);
                } else {
                    RequestMessageWindow(2, 0x6C);
                    Ovl11SetInteractionSteps(25, 9);
                }
                break;
            case 1:
            case 2:
            case 7:
            case 8:
                if (Ovl11HasSpendableBalance(Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) == 1) {
                    if (Ovl11GetItemStackCapacity(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 1) == 0) {
                        RequestMessageWindow(2, 0x54);
                        Ovl11SetInteractionSteps(25, 24);
                    } else {
                        Ovl11InsertItemStack(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), 1, 1);
                        Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
                        RequestMessageWindow(2, 0x84);
                        Ovl11SetInteractionSteps(25, 24);
                    }
                } else {
                    RequestMessageWindow(2, 0x4F);
                    Ovl11SetInteractionSteps(25, 24);
                }
                break;
            case 3:
            case 4:
                if (Ovl11FindFreeTaggedRecord() == 0) {
                    RequestMessageWindow(2, 0x72);
                    Ovl11SetInteractionSteps(25, 24);
                } else if (Ovl11HasSpendableBalance(Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord))) != 1) {
                    RequestMessageWindow(2, 0x4F);
                    Ovl11SetInteractionSteps(25, 24);
                } else {
                    RequestMessageWindow(2, 0x73);
                    Ovl11SetInteractionSteps(25, 16);
                }
                break;
            case 5:
                if (Ovl11FindFamilyRecordIndex(1, 0, -1) == 0) {
                    RequestMessageWindow(2, 0x76);
                    Ovl11SetInteractionSteps(25, 24);
                } else {
                    RequestMessageWindow(2, 0x75);
                    gOvl11InteractionDrawMode = 0;
                    Ovl11DrawAndProcessRecordFamilyList(0, 0, -1);
                    Ovl11SetInteractionSteps(25, 18);
                }
                break;
            case 6:
                if (Ovl11FindFamilyRecordIndex(2, 0, -1) == 0) {
                    RequestMessageWindow(2, 0x78);
                    Ovl11SetInteractionSteps(25, 24);
                } else {
                    RequestMessageWindow(2, 0x77);
                    gOvl11InteractionDrawMode = 0;
                    Ovl11DrawAndProcessRecordFamilyList(0, 0, -1);
                    Ovl11SetInteractionSteps(25, 21);
                }
                break;
            }
            break;
        }
        break;
    case 9:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        Ovl11SetInteractionSteps(26, 10);
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
            Ovl11SetInteractionSteps(25, 24);
            break;
        case 1:
            CloseMessageWindow();
            if (Ovl11HasSpendableBalance(state->unk8 * state->unkA) == 1) {
                if (state->unk8 + ((Ovl11SaleGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x6D);
                    Ovl11SetInteractionSteps(25, 9);
                } else {
                    ovl_11_func_801152BC(state);
                    RequestMessageWindow(2, 0x70);
                    Ovl11SetInteractionSteps(25, 24);
                }
            } else {
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(25, 9);
            }
            break;
        }
        break;
    case 16:
        ResetTextEntry();
        BeginTextEntry(D_8012D63C);
        gOvl11InteractionDrawMode = 0;
        Ovl11SetInteractionSteps(17, 0);
        break;
    case 17:
        switch (UpdateTextEntry()) {
        case -1:
            RequestMessageWindow(2, 0x6E);
            Ovl11SetInteractionSteps(25, 24);
            break;
        case 1:
            dest = Ovl11AllocateItemRecord(Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord), -1) + 2;
            for (i = 0; i < 9; i++) {
                dest[i] = D_8012D638.values[i];
            }
            Ovl11AdjustSpendableBalance(-Ovl11GetItemPrice((u16)Ovl11GetSelectionEntryId(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
            RequestMessageWindow(2, 0x74);
            Ovl11SetInteractionSteps(25, 24);
            break;
        }
        break;
    case 18:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x5A);
        Ovl11SetInteractionSteps(26, 19);
        break;
    case 19:
        selected = Ovl11DrawAndProcessRecordFamilyList(1, 0, 0);
        if (selected == -2) {
            CloseMessageWindow();
            RequestMessageWindow(2, 0x79);
            Ovl11SetInteractionSteps(25, 24);
        } else if (selected != -1) {
            CloseMessageWindow();
            SetMessagePortraitEnabled(0);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                RequestMessageWindow(2, 0x82);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                RequestMessageWindow(2, 0x7D);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                RequestMessageWindow(2, 0x7F);
            } else {
                RequestMessageWindow(2, 0x80);
            }
            Ovl11SetInteractionSteps(26, 20);
        }
        break;
    case 20:
        selected = Ovl11DrawAndProcessRecordFamilyList(1, 0, 1);
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x83);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                Ovl11AdjustSpendableBalance(8000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                Ovl11AdjustSpendableBalance(3000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                Ovl11AdjustSpendableBalance(4500, 1);
            } else {
                Ovl11AdjustSpendableBalance(5000, 1);
            }
            ovl_11_func_800DF0F8((s32)((u8 *)&gTaggedRecordBase + selected * 0xB8));
            Ovl11SetInteractionSteps(25, 24);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x79);
            Ovl11SetInteractionSteps(25, 24);
            break;
        }
        break;
    case 21:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x5A);
        Ovl11SetInteractionSteps(26, 22);
        break;
    case 22:
        selected = Ovl11DrawAndProcessRecordFamilyList(2, 0, 0);
        if (selected == -2) {
            CloseMessageWindow();
            RequestMessageWindow(2, 0x79);
            Ovl11SetInteractionSteps(25, 24);
        } else if (selected != -1) {
            CloseMessageWindow();
            SetMessagePortraitEnabled(0);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                RequestMessageWindow(2, 0x81);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                RequestMessageWindow(2, 0x7C);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                RequestMessageWindow(2, 0x7D);
            } else {
                RequestMessageWindow(2, 0x7E);
            }
            Ovl11SetInteractionSteps(26, 23);
        }
        break;
    case 23:
        selected = Ovl11DrawAndProcessRecordFamilyList(2, 0, 1);
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x83);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                Ovl11AdjustSpendableBalance(6000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                Ovl11AdjustSpendableBalance(2000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                Ovl11AdjustSpendableBalance(3000, 1);
            } else {
                Ovl11AdjustSpendableBalance(3500, 1);
            }
            ovl_11_func_800DF0F8((s32)((u8 *)&gTaggedRecordBase + selected * 0xB8));
            Ovl11SetInteractionSteps(25, 24);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x79);
            Ovl11SetInteractionSteps(25, 24);
            break;
        }
        break;
    case 24:
        RequestMessageWindow(2, 0x71);
        Ovl11SetInteractionSteps(25, 1);
        break;
    case 25:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 26:
        if (gOvl11InteractionResumeStep == 20) {
            Ovl11DrawAndProcessRecordFamilyList(1, 0, 1);
        }
        if (gOvl11InteractionResumeStep == 23) {
            Ovl11DrawAndProcessRecordFamilyList(2, 0, 1);
        }
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 27:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
