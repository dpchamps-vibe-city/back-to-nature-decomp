/* Five-record position/animation choreography. A caller-selected value slot
 * holds the phase; the exclusive ID and X/Z tables supply the participants.
 * Loop phases deliberately use the LAST participant's result, not an AND of
 * all participants' completion states. */

#include "common.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_five_record_sequence.h"
#include "ovl11_state_record_motion_limit.h"

#include "ovl11_record_action_predicate.h"

s32 Ovl11StepFiveRecordSequence(s16 phaseSlot) {
    s16 i;
    s16 phase;
    s32 result;
    s32 start;

    phase = *(s16 *)&gOvl11ValueSlots[phaseSlot];
    start = ovl_11_func_800EDF3C(gOvl11FiveRecordSequenceIds[0], 0, 0, 0) != 1;
    result = 0;
    switch (phase) {
    case 0:
        for (i = start; i < 5; i++) {
            Ovl11WriteStateRecordPosition(gOvl11FiveRecordSequenceIds[i], 0, gOvl11FiveRecordSequenceXZ[0][i], gOvl11FiveRecordSequenceXZ[1][i]);
            Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
        }
        result = 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 10, 0, 0);
        }
        break;
    case 5:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 0, 500, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 2, 2200, 6);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 11);
            }
        }
        break;
    case 6:
        result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[1], 12, 0, 0);
        break;
    case 7:
        Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[1], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[3], 12, 0, 0);
        if (result == 1) {
            phase += start;
        }
        break;
    case 8:
        Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[3], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[0], 12, 0, 0);
        break;
    case 9:
        Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[3], 5, 0, 1);
        Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[0], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[4], 12, 0, 0);
        break;
    case 10:
        Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[4], 5, 0, 1);
        result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[2], 12, 0, 0);
        break;
    case 11:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 2, 1350, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 0, 1350, 11);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
            }
        }
        break;
    case 12:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 13, 0, 0);
        }
        break;
    case 13:
    case 14:
    case 15:
        for (i = start; i < 5; i++) {
            result = Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 0);
        }
        break;
    case 16:
        for (i = start; i < 3; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 2, gOvl11FiveRecordSequenceXZ[1][i], 8);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
            }
        }
        for (; i < 5; i++) {
            result = Ovl11AnimateStateRecordMotion12(gOvl11FiveRecordSequenceIds[i], 0, gOvl11FiveRecordSequenceXZ[1][i], 6);
            if (result == 1) {
                Ovl11StateRecordAnimationCommand(gOvl11FiveRecordSequenceIds[i], 5, 0, 1);
            }
        }
        break;
    }
    if (result == 1) {
        if (phase == 16) {
            phase = 1;
        } else {
            phase++;
        }
    }
    gOvl11ValueSlots[phaseSlot] = phase;
    return 1;
}
