/*
 * Object-program facing and position-offset commands. Both use the same
 * unguarded selector: ID 0x29 is the primary work position; other IDs index
 * coordinate triplets in 0x1D4-byte records. No storage is owned here.
 */

#include "common.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_record_position_commands.h"
#include "ovl11_dominant_axis_direction.h"

s32 ovl_11_func_800E9284(s16 direction, s16 recordSelector, s32 unused2, s32 unused3);

s32 Ovl11FaceRecordTowardCommand(s16 idOrSlot, s16 resolveFlags,
                                s32 animation, s32 targetIdOrSlot) {
    s16 targetId;
    s16 recordId;
    s32 *target;
    s32 *origin;
    s32 direction;

    if (resolveFlags & 1) {
        recordId = (s16)gOvl11ValueSlots[idOrSlot];
    } else {
        recordId = idOrSlot;
    }
    if (resolveFlags & 2) {
        targetId = (s16)gOvl11ValueSlots[targetIdOrSlot];
    } else {
        targetId = (s16)targetIdOrSlot;
    }
    if (recordId == 0x29) {
        origin = gObjectWorkPosition;
    } else {
        origin = (s32 *)((u8 *)gObjectPositionRecordCoordinates + recordId * 0x1D4);
    }
    if (targetId == 0x29) {
        target = gObjectWorkPosition;
    } else {
        target = (s32 *)((u8 *)gObjectPositionRecordCoordinates + targetId * 0x1D4);
    }
    direction = Ovl11GetDominantXZDirection(target, origin);
    if (recordId == 0x29) {
        ovl_11_func_800E9284((s16)direction, 0, 0, 0);
    } else {
        Ovl11StateRecordAnimationCommand(recordId, (s16)animation, direction, 1);
        Ovl11ChangeStateRecordFlag800(recordId, 0, 0, 0);
    }
    return 1;
}

s32 Ovl11OffsetRecordPositionCommand(s16 recordId, s16 yDelta,
                                    s32 xDelta, s32 zDelta) {
    s32 *position;

    if (recordId == 0x29) {
        position = gObjectWorkPosition;
    } else {
        position = (s32 *)((char *)gObjectPositionRecordCoordinates + recordId * 0x1D4);
    }
    position[1] += yDelta * 2;
    position[0] += xDelta * 2;
    position[2] += zDelta * 2;
    return 1;
}
