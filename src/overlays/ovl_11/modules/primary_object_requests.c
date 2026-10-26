/*
 * Primary-object requests: reset to state 1, install a three-halfword request,
 * and handle an emptied selected slot. Both installation paths replace the
 * same +0xF0 triplet before selecting a state/animation. Completion can call
 * the reset path and otherwise clears the installer's 0x8000 flag.
 *
 * The object and item table are shared engine state, not private storage.
 * The specialized request/flag protocol and internal reset call, corroborated
 * by original order, support this four-entry unit (notes/file-groupings.md).
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_halfword_triplets.h"
#include "sound_commands.h"
#include "ovl11_primary_object_state_change.h"

s32 ovl_11_func_800D6A94(s16 id);
void ovl_11_func_800C9D98(void);

s32 Ovl11ResetPrimaryObjectRequest(void) {
    s32 *work;

    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 1U, 0, -2);
    /* Retain the derived work-area base: its accesses alias object +26/+6C. */
    work = &gPrimaryObjectBase - 0x1472;
    (*(s16 *) ((u8 *) work + 0x51EE)) = 0;
    (*(s32 *) ((u8 *) work + 0x5234)) = (s32)
        (((*(s32 *) ((u8 *) work + 0x5234)) & 0xDFFFFFFF) | 0x04000000);
    return 0;
}

s32 Ovl11InstallPrimaryObjectRequest(u16 *request) {
    Ovl11C7010Object *object = (Ovl11C7010Object *) &gPrimaryObjectBase;
    s32 *storedRequest;
    s32 *work;
    s32 animationBase;
    u16 state;

    storedRequest = (s32 *) ((u8 *) object + 0xF0);
    Ovl11ClearHalfwordTriplet((s16 *) storedRequest);
    state = 6;
    work = (s32 *) ((u8 *) object - 0x51C8);
    animationBase = 0xB;
    if ((*(ItemData **) ((u8 *) work + 0x20))[(s16) *request].u0.flags & 0x100) {
        state = 0xE;
        animationBase = 0x25;
    }
    Ovl11CopyHalfwordTriplet((u16 *) storedRequest, request);
    ovl_11_func_800D6A94(*(s16 *) ((u8 *) object + 0xF0));
    Ovl11ChangePrimaryObjectState(object, state,
        animationBase + (*(u16 *) ((u8 *) work + 0x5200)), -2);
    object->unk6C |= 0x8004;
    return 0;
}

void Ovl11CheckPrimaryObjectRequestSlot(void) {
    s32 index;
    Ovl11C7010Object *object = (Ovl11C7010Object *) &gPrimaryObjectBase;
    Ovl11CB618RecordView *slots;

    if (!(object->unk6C & 0x10000000)) {
        Ovl11ResetPrimaryObjectRequest();
        return;
    }
    ovl_11_func_800C9D98();
    index = *(s16 *) ((u8 *) object + 0x20) - 1;
    slots = (Ovl11CB618RecordView *) object;
    if (slots->records[index][0] == 0) {
        PlaySoundEffect(8);
        Ovl11ChangePrimaryObjectState(object, 0xCU, object->unk38 + 0x1D, -2);
        object->unk6C &= ~0x8000;
    }
}

void Ovl11InstallPrimaryObjectState30Request(u16 *request) {
    Ovl11C7010Object *object;
    u16 *storedRequest;

    object = (Ovl11C7010Object *) &gPrimaryObjectBase;
    storedRequest = (u16 *) ((u8 *) object + 0xF0);
    Ovl11ClearHalfwordTriplet((s16 *) storedRequest);
    Ovl11CopyHalfwordTriplet(storedRequest, request);
    Ovl11ChangePrimaryObjectState(object, 0x30U, object->unk38 + 0x21, -2);
}
