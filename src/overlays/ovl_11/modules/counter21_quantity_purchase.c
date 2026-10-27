/* Save-counter-21 quantity purchase (ovl_11:80118CE4).
 *
 * Interaction-table s32(void) callback: one call per update, 0 pending,
 * 1 finished. Requires resident ovl_11, initialized render/message/request
 * services and a quantity prompt with a valid unit value. Global steps,
 * quantity and draw state make it non-reentrant; this is not an atomic API.
 *
 * Entry disables dispatch record 84 and submits item 6C before any dialogue.
 * Quantity editing caps at 999 and rejects sums above counter 21's 999 limit.
 * Only confirmation adds the quantity and debits quantity*unitValue. Both
 * acceptance and cancellation re-enable record 84 and attempt to remove 6C
 * from the secondary single slot; those service return values are ignored.
 * Entry clears work+522C/+5230 and asserts +5234 bit 800000; exit clears it.
 * Numeric IDs are retained because their gameplay identities are unverified.
 * No direct C clients: the callback bank is the public interface.
 * Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_primary_object_wait_steps.h"
#include "ovl11_save_counters.h"
#include "ovl11_balance_accounting.h"
#include "ovl11_quantity_input.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_dispatch_record_enable.h"

void SetVal8005E2BC(s32 arg0);

s32 Ovl11UpdateCounter21Purchase(void) {
    s32 result;

    result = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230 = 0;
    ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        Ovl11SetInteractionSteps(9, 0);
        Ovl11SetDispatchRecordEnabled(0x84, 0);
        break;
    case 9:
    case 10:
        if (Ovl11StepPrimaryObjectItemRequest(&gOvl11InteractionStep, 9, 0x6C)) {
            Ovl11SetInteractionSteps(1, 0);
        }
        break;
    case 1:
        if (((Ovl11FB908State *)D_8006C838)->saveCounter21 >= 999) {
            RequestMessageWindow(2, 0xD1);
            Ovl11SetInteractionSteps(2, 7);
        } else {
            RequestMessageWindow(2, 0xD0);
            Ovl11SetInteractionSteps(2, 4);
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        if (GetMessageWindowStatus() == 5) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 4:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        Ovl11SetInteractionSteps(3, 5);
        break;
    case 5:
        gOvl11InteractionDrawMode = 1;
        switch (Ovl11ProcessQuantityInput(999, (u16 *)&gOvl11QuantityPrompt.unk8)) {
        case 0:
            break;
        case 1:
            if (Ovl11HasSpendableBalance(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA) == 1) {
                if (gOvl11QuantityPrompt.unk8 + ((Ovl11FB908State *)D_8006C838)->saveCounter21 >= 1000) {
                    CloseMessageWindow();
                    RequestMessageWindow(2, 0xD2);
                    Ovl11SetInteractionSteps(2, 4);
                } else {
                    CloseMessageWindow();
                    RequestMessageWindow(2, 0xD3);
                    Ovl11SetInteractionSteps(2, 6);
                }
            } else {
                CloseMessageWindow();
                RequestMessageWindow(2, 0x4F);
                Ovl11SetInteractionSteps(2, 4);
            }
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x25B);
            Ovl11SetInteractionSteps(2, 4);
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(2, 0xBA);
            Ovl11SetInteractionSteps(2, 7);
            break;
        }
        break;
    case 6:
        Ovl11AddSaveCounter21(1, (u16)gOvl11QuantityPrompt.unk8);
        Ovl11AdjustSpendableBalance(-(gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA), 1);
        /* Commit and run the shared cancellation/success cleanup immediately. */
    case 7:
        Ovl11SetDispatchRecordEnabled(0x84, 1);
        Ovl11MatchItemSlotsCommand(0x6C, 1, 0, 1);
        Ovl11SetInteractionSteps(8, 0);
        break;
    case 8:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
