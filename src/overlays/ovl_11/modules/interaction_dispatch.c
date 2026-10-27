/*
 * Begin/update dispatch for the table-driven interaction protocol
 * (ovl_11:801189C8–80118B84). These are the callback bank's two readers.
 * Begin[1] and the completion reset are two views of the SAME word at
 * 800BB2C0; the four-byte begin anchor precedes it at 800BB2BC.
 *
 * Keep the established scalar table-anchor views and indirect-call ABIs.
 * Drawing passes the draw-bank base, even after an update resets the ID.
 * Public return-value and preflight constraints: ovl11_interaction_dispatch.h.
 */

#include "common.h"
#include "game_flags.h"
#include "ovl11_interaction_dispatch.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_condition_evaluation.h"

s32 Ovl11UpdateInteractionDispatch(void) {
    s32 (*updateOrDraw)(void);
    s32 (*reset)(void);
    s32 *drawBank;
    s32 complete;

    complete = 1;
    if (gOvl11InteractionId != 0) {
        updateOrDraw = (s32 (*)(void))((s32 *)&gOvl11InteractionUpdateCallbacks)[gOvl11InteractionId];
        if (updateOrDraw != 0) {
            complete = 0;
            if (updateOrDraw() != 0) {
                complete = 1;
                reset = (s32 (*)(void))gOvl11InteractionResetCallback;
                reset();
            }
        }
        drawBank = &gOvl11InteractionDrawCallbacks;
        updateOrDraw = (s32 (*)(void))drawBank[gOvl11InteractionDrawMode];
        if (updateOrDraw != 0) {
            ((void (*)(s32 *))updateOrDraw)(drawBank);
        }
    }
    return complete;
}

s32 Ovl11BeginInteraction(s32 interactionId) {
    s32 (*begin)(void);
    s32 result = 1;

    if ((gOvl11InteractionId == 0) || (interactionId == 1)) {
        if ((interactionId == 0x16) && (GetGameFlag(2U) == 0)) {
            Ovl11SetConditionState(0xA);
            Ovl11EvaluatePendingConditions();
            Ovl11ServiceActiveRecordSlot();
            if (GetGameFlag(2U) == 1) {
                do {
                    if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                        Ovl11ServiceActiveRecordSlot();
                        break;
                    }
                    Ovl11ServiceActiveRecordSlot();
                } while (GetGameFlag(2U) != 0);
            }
            if (GetGameFlag(2U) == 1) {
                return 0;
            }
        }
        begin = gOvl11InteractionBeginCallbacks[interactionId];
        gOvl11InteractionId = interactionId;
        result = 0;
        if (begin != 0) {
            begin();
        }
    }
    return result;
}
