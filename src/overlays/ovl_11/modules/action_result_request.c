/*
 * Action-result resolution and sprite-rotation requests.
 *
 * Five shared override slots take precedence over the action-guard service.
 * Without a usable result, or when explicitly requested, delegate to the
 * registered sprite-rotation pool. See ovl11_action_result_request.h for the
 * asymmetric output-pointer and return-value contract.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "sprite_rotation.h"
#include "ovl11_action_guards.h"
#include "game_flags.h"
#include "ovl11_action_result_request.h"

s32 Ovl11ResolveActionResultOrRotate(s16 actionId, u16 direction,
                                    s32 *resultOut, s32 forceRotation) {
    s16 *slot;
    s16 overrideId;
    s16 overrideResult;
    s32 slotIndex;
    s32 guardResult;

    guardResult = -1;
    overrideId = -1;
    overrideResult = 0;
    slot = (s16 *)gActionResultOverrides;
    for (slotIndex = 0; slotIndex < 5; slotIndex++) {
        if ((slot[0] == actionId) && (actionId != -1)) {
            overrideId = actionId;
            overrideResult = slot[1];
        }
        slot += 2;
    }
    if (overrideId == -1) {
        if (forceRotation == 0) {
            guardResult = Ovl11FindActionGuardResult((s32)actionId);
        }
        /* This override deliberately precedes the global suppression flag
         * and writes through resultOut without a NULL check. */
        if ((actionId == 0x20) && (GetGameFlag(0x76U) == 1)) {
            *resultOut = 0x209;
            goto result_ready;
        }
        if (D_8006C848 & 0x04000000) {
            guardResult = -1;
        }
        if (guardResult != -1) {
            if (resultOut != NULL) {
                *resultOut = guardResult;
                if (Ovl11GetSpriteRotationState((s32)actionId) != 1) {
                    *resultOut = -1;
                    return 0;
                }
            }
            return 0;
        }
        goto request_rotation;
    }
    if (forceRotation != 0) {
request_rotation:
        return Ovl11RequestSpriteRotation(actionId, direction & 0xFFFF);
    }
    if (resultOut != NULL) {
        *resultOut = (s32)overrideResult;
result_ready:
        ;
    }
    return 0;
}
