/*
 * Pending-record selection and its orientation/animation worker.
 * The commit entry directly uses the adjacent worker; the public contract
 * is in ovl11_pending_record_animation.h. Original order: E648C, E65AC.
 * Shared work/value slots stay external; this is not a new storage owner.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "game_flags.h"
#include "psyq/libgte.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_pending_record_animation.h"
#include "psyq/abs.h"
#include "ovl11_camera_position_control.h"
#include "ovl11_state_value_operand_commands.h"

s32 ovl_11_func_800F06D8(s32 *arg0, s32 *arg1);

s32 Ovl11CommitPendingRecordSelection(s16 resultSlot, s16 animationPlusOne, s32 updateField, s32 requireFlag46) {
    s16 pendingRecord;
    s32 accepted;

    accepted = 0;
    pendingRecord = ((struct Ovl11PendingSelectionWorkView *)D_8006C838)->pendingRecord;
    if (pendingRecord != -1) {
        if (resultSlot != -1) {
            gOvl11ValueSlots[resultSlot] = pendingRecord;
        }
        accepted = 1;
        if (requireFlag46 != 0) {
            accepted = GetGameFlag(0x46U);
        }
        if (accepted == 1) {
            if (animationPlusOne != 0) {
                Ovl11OrientAndAnimateStateRecord(pendingRecord, 0, animationPlusOne - 1, 0);
            }
            if (updateField != 0) {
                Ovl11UpdateStateFieldFromLiteral(0x2C, pendingRecord, 1, 0);
            }
        }
    }
    if ((requireFlag46 == 0) || (accepted == 1)) {
        ((struct Ovl11PendingSelectionWorkView *)D_8006C838)->pendingRecord = -1;
        SetGameFlag(0x46U, 0U);
    }
    Ovl11SetPositionControlFlags(accepted == 0, 1, 0, 0);
    return accepted;
}

s32 Ovl11OrientAndAnimateStateRecord(s16 recordOrSlot, s32 resolveSlot, s32 animation, s32 recordSpace) {
    s16 selectedAnimation;
    s32 *position;
    s32 direction;
    s32 recordIndex;
    u8 state;

    selectedAnimation = animation;
    if ((resolveSlot << 16) != 0) {
        recordIndex = gOvl11ValueSlots[recordOrSlot];
    } else {
        recordIndex = recordOrSlot;
    }
    if (recordSpace == 1) {
        position = (s32 *)((char *)gCompactObjectPositionCoordinates + recordIndex * 0xF8);
    } else {
        position = (s32 *)((char *)gObjectPositionRecordCoordinates + recordIndex * 0x1D4);
    }
    direction = ovl_11_func_800F06D8(gObjectWorkPosition, position);
    switch (recordSpace) {
    case 1:
        ((struct Ovl11CompactRecordDirectionView *)((u8 *)gObjectWorkPosition - 0x52C8 + recordIndex * 0xF8))->direction = (s16)direction;
        goto done;
    case 2:
        state = ((struct Ovl11RecordAnimationStateView *)((u8 *)gObjectWorkPosition - 0x52C8 + recordIndex * 0x1D4))->animationState;
        if ((u32)(state - 5) < 5U) {
            selectedAnimation = 5;
        } else if (state == 0xA) {
            selectedAnimation = 0xB;
            direction = 0;
        }
        break;
    }
    Ovl11StateRecordAnimationCommand((s16)(recordIndex << 16 >> 16), selectedAnimation, direction, 1);
    Ovl11ChangeStateRecordFlag800((s16)(recordIndex << 16 >> 16), 0, 0, 0);
done:
    return 1;
}
