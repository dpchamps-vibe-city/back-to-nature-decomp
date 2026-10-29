/* Two-stage selection purchase callback (ovl_11:80119DF8).
 *
 * Interaction-table s32(void) interface: call once per update, returning 0
 * while pending and 1 on exit. Requires resident ovl_11, initialized messages/
 * rendering and valid primary/secondary selection records. Global interaction
 * steps and draw mode make it non-reentrant; selections must remain stable.
 *
 * Primary indices 1..4 delegate the selected-item transaction. Index 5 opens
 * secondary selection only when shared flags 1C0000 permit it; index 0 reports
 * rejection. Secondary acceptance checks capacity then balance, debits its
 * current price, inserts one item and refreshes progress and selection state.
 * Cancellation returns to the primary menu or exits according to prompt phase.
 * Each call clears work+522C/+5230 and sets +5234 bit 800000; exit clears it.
 * Descriptors retain their by-value ABI, partial initialization and store order.
 * Callback-table entry is the public interface, not a new direct-call header.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_progress_status.h"
#include "ovl11_item_stack_insert.h"
#include "ovl11_selection_record_access.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"
#include "psyq/stddef.h"
#include "ovl11_prompt_option_rows.h"

s32 Ovl11UpdateTwoStageSelectionPurchase(void) {
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
    price = Ovl11GetItemPrice(item);
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
        /* Retry the first prompt without a one-update gap. */
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
            } else if (Ovl11HasSpendableBalance(price) == 1) {
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
    /* The descriptor's leading pointer is the selection-record argument. */
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
        Ovl11AdjustSpendableBalance(-price, 1);
        Ovl11InsertItemStack(item, 1, 1);
        Ovl11RefreshProgressStatus();
        Ovl11FillPrimaryAndSecondaryPromptRows();
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
