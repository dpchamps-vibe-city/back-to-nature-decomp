/*
 * Edge-triggered level-band flags for the primary object.
 *
 * The two signed levels at +12 and +16 share the unsigned flag word at +36.
 * Narrower bands inhibit the wider ones, so repeated calls do not restart a
 * state while a band remains active. See ovl11_primary_object_level_bands.h.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_level_bands.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_active_record_slot_drain.h"

s32 Ovl11UpdatePrimaryObjectLevelBands(Ovl11C7010Object *object, s32 suppressStateChange) {
    s32 tenth;
    s32 maximum;
    s32 changedState;
    u16 zeroFlags;
    u16 tenthFlags;
    u16 threeTenthsFlags;
    u16 halfFlags;
    u16 fullFlags;
    u16 highFlags;
    u16 middleFlags;
    u16 nextState;

    nextState = 0;
    maximum = (s16) *(u16 *)((u8 *)object + 0x14);
    tenth = maximum / 10;
    if (*(s16 *)((u8 *)object + 0x12) == 0) {
        zeroFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(zeroFlags & 0x1000)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(zeroFlags | 0x1000);
            nextState = 0x40;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xEFFF);
    }
    if (*(s16 *)((u8 *)object + 0x12) < tenth) {
        tenthFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(tenthFlags & 0x3000)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(tenthFlags | 0x2000);
            nextState = 0x3F;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xCFFF);
    }
    if (*(s16 *)((u8 *)object + 0x12) < tenth * 3) {
        threeTenthsFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(threeTenthsFlags & 0x7000)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(threeTenthsFlags | 0x4000);
            nextState = 0x3E;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0x8FFF);
    }
    if (*(s16 *)((u8 *)object + 0x12) < tenth * 5) {
        halfFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(halfFlags & 0xF000)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(halfFlags | 0x8000);
            nextState = 0x3D;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xFFF);
    }
    if (*(s16 *)((u8 *)object + 0x16) == 0x64) {
        fullFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(fullFlags & 0x200)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(fullFlags | 0x200);
            nextState = 0x40;
            Ovl11DrainActiveRecordSlot();
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xFDFF);
    }
    if (*(s16 *)((u8 *)object + 0x16) >= 0x50) {
        highFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(highFlags & 0x600)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(highFlags | 0x400);
            nextState = 0x43;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xF9FF);
    }
    if (*(s16 *)((u8 *)object + 0x16) >= 0x32) {
        middleFlags = *(u16 *)((u8 *)object + 0x36);
        if (!(middleFlags & 0xE00)) {
            *(u16 *)((u8 *)object + 0x36) = (u16)(middleFlags | 0x800);
            nextState = 0x42;
        }
    } else {
        *(u16 *)((u8 *)object + 0x36) = (u16)(*(u16 *)((u8 *)object + 0x36) & 0xF1FF);
    }
    changedState = 0;
    if ((suppressStateChange == 0) && (nextState != 0)) {
        object->unk6C |= 0x5800;
        Ovl11ChangePrimaryObjectState(object, nextState, 0, -2);
        changedState = 1;
    }
    return changedState;
}
