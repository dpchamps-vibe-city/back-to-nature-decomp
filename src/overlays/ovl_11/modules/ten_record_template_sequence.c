/*
 * ten_record_template_sequence — private ID/position/frame choreography.
 * The aggregate copy and two-by-five row-major view preserve the original
 * template emission/access protocol. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_record_action_predicate.h"
#include "ovl11_ten_record_template_sequence.h"
#include "ovl11_record_approach_sequence.h"

s32 Ovl11StepTenRecordTemplateSequence(s16 phaseSlot, s16 templateSlot) {
    u16 value = *(u16 *)&gOvl11ValueSlots[templateSlot];
    struct Ovl11TenRecordIdTemplate ids = gOvl11TenRecordIdTemplate;
    u32 state;
    s32 selection;
    s32 i;
    s16 *recordId;
    s32 *positionsX;
    s32 *positionsZ;
    s16 *frames;

    ids.values[1][4] = value;
    switch ((s16)(value - 3)) {
    case 17: selection = 0; break;
    case 0: selection = 1; break;
    case 11: selection = 2; break;
    case 4: selection = 3; break;
    case 6: selection = -1; break;
    default: selection = 4; break;
    }
    if (selection != -1) {
        Ovl11ChangeStateRecordFlag800(ids.values[0][selection], 1, 0, 0);
        ids.values[0][selection] = 8;
        ids.values[1][selection] = 9;
    } else {
        Ovl11ChangeStateRecordFlag800(8, 1, 0, 0);
    }
    if (Ovl11IsRecordActionAllowed(11, 0, 0, 0) == 0 && ids.values[1][4] != 7) {
        Ovl11ChangeStateRecordFlag800(ids.values[0][3], 1, 0, 0);
        ids.values[0][3] = -1;
        ids.values[1][3] = -1;
    }
    if (Ovl11IsRecordActionAllowed(ids.values[1][0], 0, 0, 0) == 0) {
        Ovl11ChangeStateRecordFlag800(ids.values[0][0], 1, 0, 0);
        ids.values[0][0] = -1;
        ids.values[1][0] = -1;
    }
    recordId = (s16 *)&ids;
    positionsX = gOvl11TenRecordTemplateX;
    positionsZ = gOvl11TenRecordTemplateZ;
    frames = gOvl11TenRecordTemplateFrames;
    if (gOvl11ValueSlots[phaseSlot] == -1) {
        for (i = 0; i < 10;) {
            if (*recordId != -1) {
                Ovl11WriteStateRecordPosition(*recordId, 0, *positionsX, *positionsZ);
                Ovl11StateRecordAnimationCommand(*recordId, 0, *frames, 1);
            }
            i++;
            recordId++;
            positionsX++;
            positionsZ++;
            frames++;
        }
        gOvl11ValueSlots[phaseSlot]++;
    } else {
        for (i = 0; i < 10;) {
            if (*recordId != -1) {
                /* Each record starts from the published phase; last result wins. */
                state = gOvl11ValueSlots[phaseSlot];
                Ovl11StepRecordApproachSequence(*recordId, *positionsX, *positionsZ, &state);
            }
            i++;
            recordId++;
            positionsX++;
            positionsZ++;
        }
        gOvl11ValueSlots[phaseSlot] = state;
    }
    return 1;
}
