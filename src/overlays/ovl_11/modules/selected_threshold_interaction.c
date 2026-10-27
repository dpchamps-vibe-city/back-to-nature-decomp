/* Message-driven confirmation of a selected entry's signed threshold.
 * The predicate and debit deliberately use two address views of the same
 * balance word (8006C838 + 5228 == 80071A60). No storage is owned here.
 * Membership and conservative protocol boundaries: notes/file-groupings.md. */

#include "selected_threshold_interaction.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
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
#include "ovl11_held_item_replacement.h"
#include "two_choice_prompt.h"
#include "ovl11_item_selection_value.h"

void SetVal8005E2BC(s32 arg0);
s32 Ovl11CanApplySelectedThreshold(StructD548 *arg0);
void Ovl11ApplySelectedThreshold(StructD548 *arg0);

s32 Ovl11UpdateThresholdInteraction(void) {
    Ovl11D8012D7B8 params;
    s32 complete;

    complete = 0;
    params.unk4 = -1;
    params.unk8 = -1;
    params.unk10 = 7;
    params.unk0 = gOvl11PrimarySelectionRecord;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        gOvl11InteractionDrawMode = 7;
        Ovl11SetInteractionSteps(1, 0);
        params.unk4 = -1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        switch (Ovl11StepSelectionPrompt(&gOvl11InteractionStep, 1, params)) {
        case 0:
            break;
        case 3:
            RequestMessageWindow(1, 0x447);
            Ovl11SetInteractionSteps(9, 11);
            break;
        case 1:
            if (Ovl11CanApplySelectedThreshold((StructD548 *)gOvl11PrimarySelectionRecord) == 1) {
                RequestMessageWindow(1, 0x449);
                Ovl11SetInteractionSteps(10, 8);
            } else {
                RequestMessageWindow(1, 0x44A);
                Ovl11SetInteractionSteps(9, 0);
            }
            break;
        case 2:
            break;
        }
        break;
    case 8:
        switch (Ovl11UpdateCompactChoicePrompt(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(1, 0x448);
            Ovl11ApplySelectedThreshold((StructD548 *)gOvl11PrimarySelectionRecord);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(1, 0x447);
            Ovl11SetInteractionSteps(9, 0);
            break;
        }
        break;
    case 10:
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            Ovl11ResumeInteractionStep();
        }
        break;
    case 9:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 11:
        complete = 1;
        break;
    case 12:
    case 13:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 12, ((StructD548 *)gOvl11PrimarySelectionRecord)->data[((StructD548 *)gOvl11PrimarySelectionRecord)->index])) {
            Ovl11SetInteractionSteps(11, 0);
        }
        break;
    }
    return complete;
}

/* Lookup results are narrowed to s16 before either comparison or debit. */
s32 Ovl11CanApplySelectedThreshold(StructD548 *record) {
    s16 threshold;

    threshold = Ovl11LookupItemSelectionValue((u16) record->data[record->index]);
    return gSelectedThresholdBalance >= threshold;
}

/* The caller checks eligibility first; this operation does not clamp or
 * repeat the balance test. The selector also chooses the completion action. */
void Ovl11ApplySelectedThreshold(StructD548 *record) {
    s16 selection;
    s16 threshold;

    gOvl11InteractionDrawMode = 0;
    threshold = Ovl11LookupItemSelectionValue((u16)record->data[record->index]);
    ((Ovl11SelectedThresholdState *)D_8006C838)->balance -= threshold;
    selection = record->index;
    if (selection == 0) {
        ((Ovl11SelectedThresholdState *)D_8006C838)->flags |= 0x10;
        Ovl11ReplaceHeldItem((Ovl11C7010Object *)((Ovl11SelectedThresholdState *)D_8006C838)->object, 0x67, 2);
        Ovl11SetInteractionSteps(9, 0xB);
    } else if (selection == 1) {
        Ovl11AddSaveCounter19(3, 0x1F4);
        Ovl11SetInteractionSteps(9, 0xB);
    } else {
        Ovl11SetInteractionSteps(9, 0xC);
    }
}
