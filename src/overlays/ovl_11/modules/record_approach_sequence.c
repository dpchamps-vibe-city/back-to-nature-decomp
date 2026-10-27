/* Ten-phase animation/position service, ovl_11:800F07EC.
 * Both the slot adapter and the ten-record sequencer use this public service.
 * Contract: ovl11_record_approach_sequence.h.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_record_approach_sequence.h"
#include "ovl11_object_events.h"
#include "ovl11_state_record_commands.h"

s32 Ovl11StepRecordApproachSequence(s16 recordId, s32 targetX, s32 targetZ,
                                    u32 *phase) {
    s16 animation;
    s32 isSpecial;
    s32 zEnd;
    s32 xEnd;
    s32 completed;
    u32 currentPhase;
    s32 *position;

    position = (s32 *) ((u8 *) gObjectPositionRecordCoordinates + (recordId * 0x1D4));
    isSpecial = Ovl11IsSpecialCounterSlot((s32) recordId);
    currentPhase = *phase;
    completed = 0;
    switch (currentPhase) {
    case 0:
    case 1:
    case 2:
    case 3:
        completed = Ovl11StateRecordAnimationCommand(recordId, 5, 0, 0);
        break;
    case 4:
        Ovl11StateRecordAnimationCommand(recordId, 6, 0, 0);
        zEnd = targetZ + 0x258;
        position[2] += 0xA;
        if (zEnd < position[2]) {
            position[2] = zEnd;
            completed = 1;
        }
        break;
    case 5:
        Ovl11StateRecordAnimationCommand(recordId, 6, 0, 0);
        position[2] -= 0xA;
        if (position[2] < targetZ) {
            position[2] = targetZ;
            completed = 1;
        }
        break;
    case 6:
        if (isSpecial == 0) {
            Ovl11StateRecordAnimationCommand(recordId, 4, 0, 0);
            xEnd = targetX + 0x1F4;
            position[0] += 0xA;
            if (xEnd < position[0]) {
                position[0] = xEnd;
                completed = 1;
            }
        } else {
            Ovl11StateRecordAnimationCommand(recordId, 2, 0, 0);
            xEnd = targetX - 0x1F4;
            position[0] -= 0xA;
            if (position[0] < xEnd) {
                position[0] = xEnd;
                completed = 1;
            }
        }
        break;
    case 7:
        completed = Ovl11StateRecordAnimationCommand(recordId, 8, 0, 0);
        break;
    case 8:
        if (isSpecial == 0) {
            Ovl11StateRecordAnimationCommand(recordId, 2, 0, 0);
            position[0] -= 0xA;
            if (position[0] < targetX) {
                position[0] = targetX;
                completed = 1;
            }
        } else {
            Ovl11StateRecordAnimationCommand(recordId, 4, 0, 0);
            position[0] += 0xA;
            if (targetX < position[0]) {
                position[0] = targetX;
                completed = 1;
            }
        }
        break;
    case 9:
        animation = 7;
        if (isSpecial == 0) {
            animation = 8;
        }
        completed = Ovl11StateRecordAnimationCommand(recordId, animation, 0, 0);
        break;
    }
    if (completed == 1) {
        if (*phase == 9) {
            *phase = 0;
        } else {
            *phase += 1;
            completed = 0;
        }
    }
    return completed;
}
