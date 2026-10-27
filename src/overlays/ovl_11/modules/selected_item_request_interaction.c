/* Selected-item request purchase callback (ovl_11:80119AF0).
 *
 * Interaction-table s32(void) interface: call once per update, returning 0
 * while waiting and 1 when finished/cancelled. Requires resident ovl_11,
 * initialized messages/render context and a valid primary selection record.
 * Shares global step/resume/draw state; not reentrant. The selected ID and
 * current price are read at acceptance rather than latched at prompt start.
 *
 * Successful selection debits once, shows the accepted message, submits the
 * primary-object item request, then waits for its completion flags. Insufficient
 * funds return to selection; cancellation finishes without a request. The
 * downstream request helper does not retry a rejected submission.
 * Every call clears work+522C/+5230 and sets +5234 bit 800000; unlike the
 * adjacent interactions, this callback does NOT clear that bit on completion.
 * Its callback-table entry is the public interface; linkage remains external.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_selection_record_access.h"
#include "ovl11_selection_prompt_steps.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_cell_grid_lookup.h"

s32 Ovl11UpdateSelectedItemRequestPurchase(void) {
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
        /* Retry with the new message and process its first phase immediately. */
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
