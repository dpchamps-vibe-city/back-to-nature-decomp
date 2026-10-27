/*
 * State entry and frame dispatch for the 21-slot object callback protocol.
 * Callback addresses remain in their original rodata image, not C-owned
 * storage. All entry points retain external linkage; the handler bank and
 * object-ID chooser are services outside this evidence-supported core.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "ovl11_object_state_dispatch.h"
#include "game_types.h"
#include "ovl11_object_id_state.h"
#include "ovl11_direction_vector.h"

s32 Ovl11UpdateObjectState(u16 *record) {
    s32 (*frameCallback)(u16 *);

    if (*record == 0) {
        return -1;
    }
    Ovl11RefreshObjectSpriteForId(record);
    if (*record == 0) {
        return -1;
    }
    frameCallback = (s32 (*)(u16 *)) gOvl11ObjectStateFrameCallbacks[*(u16 *) ((u8 *) record + 0x26)];
    if (frameCallback != 0) {
        frameCallback(record);
    }
    if (!(*(s32 *) ((u8 *) record + 0x34) & 0x800) &&
        *(s16 *) ((u8 *) record + 0x2C) >= 300) {
        Ovl11EnterObjectStateForId(record);
        *(s16 *) ((u8 *) record + 0x2C) = 0;
    }
    (*(s16 *) ((u8 *) record + 0x2C))++;
    return 0;
}

s32 Ovl11EnterObjectState(u16 *record, s32 state) {
    s32 gateResult;

    gateResult = 0;
    if (gOvl11ObjectStateEntryCallbacks[state] != 0) {
        if (*(s16 *) ((u8 *) record + 0x26) == state) {
            if (!(*(s32 *) ((u8 *) record + 0x34) & 0x800) || (gateResult = -1, state == 6)) {
                goto run_entry_gate;
            }
        } else {
run_entry_gate:
            gateResult = gOvl11ObjectStateEntryCallbacks[state](record);
        }
    }
    if (gOvl11ObjectStateFrameCallbacks[state] != 0 && gateResult != -1) {
        *(s16 *) ((u8 *) record + 0x26) = state;
        *(s16 *) ((u8 *) record + 0x28) = 0;
        *(s16 *) ((u8 *) record + 0x2A) = 0;
        *(s16 *) ((u8 *) record + 0x2C) = 0;
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) record + 0x48), 0);
    }
    return gateResult;
}
