/*
 * Convert the latched interaction hit into an item request and gate commitment
 * on the primary object's input/action flags. The selected-item resolver is an
 * external service; it writes a six-byte result and consumes the hit latch.
 * See ovl11_item_request_actions.h and notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_item_request_actions.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_item_action_gate.h"

s32 ovl_11_func_800C7878(M2C_15512f31231d_Func800C7878Arg0 *object,
                       M2C_15512f31231d_F413CArg1 *result);

s32 Ovl11CommitSelectedItemRequest(Ovl11C7010Object *object) {
    s16 request[3];

    /* Only the ID is initialized: the resolver fills the remaining halfwords
     * when it produces a nonzero ID. Keep the original halfword storage. */
    request[0] = 0;
    ovl_11_func_800C7878((M2C_15512f31231d_Func800C7878Arg0 *)object,
                       (M2C_15512f31231d_F413CArg1 *)request);
    if (request[0] != 0) {
        Ovl11InstallPrimaryObjectRequest((u16 *)request);
        object->unk6C &= ~0x200000;
        return 1;
    }
    return 0;
}

s32 Ovl11DispatchItemRequestInput(Ovl11C7010Object *object, u16 pressed) {
    if (pressed & 0x40) {
        if (!(object->unk6C & 0x18000000)) {
            if ((gOvl11InteractionHitFlags & 0x2000) &&
                Ovl11CommitSelectedItemRequest(object) != 0) {
                return 1;
            }
        } else if (Ovl11TryBeginItemAction(0) != 0) {
            return 1;
        }
    }
    return 0;
}
