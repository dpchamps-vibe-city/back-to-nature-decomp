/*
 * Slot-driven scored scene sequence, ovl_11:800EC52C–800ED4A8.
 *
 * The opcode entry owns the complete phase machine and its switch tables.
 * Item admission, randomized record selection and the two staging sequences
 * share four caller-supplied value slots. Positions are raw scene coordinates.
 * See ovl11_scored_scene_sequence.h for phase and completion semantics, and
 * notes/file-groupings.md for membership and evidence-limited boundaries.
 */
#include "common.h"
#include "ovl11_scored_scene_sequence.h"
#include "ovl11_entity_pool_access.h"
#include "random.h"
#include "ovl11_work_position_commands.h"
#include "ovl11_selection_candidate.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_state_record_motion_limit.h"
#include "ovl11_message_command.h"
#include "ovl11_scene_sound_commands.h"
#include "ovl11_camera_position_control.h"
#include "ovl11_record_action_predicate.h"
#include "ovl11_camera_record_selection.h"
#include "ovl11_master_volume_ramp_command.h"
#include "ovl11_state_field_broadcast_command.h"
#include "ovl11_clock_advance_command.h"
#include "ovl11_object_state_selection_command.h"
#include "ovl11_cd_audio_command.h"

s32 ovl_11_func_800F0A58(s16);

s32 Ovl11StepScoredSceneSequence(s16 phaseSlot, s16 itemIdSlot,
                       s32 itemValueSlot, s32 selectedRecordSlot) {
    /* Randomization mutates only this per-call copy, not the base table. */
    struct Ovl11SceneScores scores = gOvl11SceneSequenceBaseScores;
    s32 phase;
    s32 nextPhase;
    s32 result;
    s32 scoreRow;
    s32 best;
    s16 i;
    s32 x;
    s32 z;
    s32 message;
    s16 *item;

    if (phaseSlot == -1) {
        Ovl11ActivateEntityAtPosition(0, -200, -1227, 1402);
        Ovl11ActivateEntityAtPosition(2, -200, -653, 1402);
        Ovl11ActivateEntityAtPosition(1, -200, -996, -999);
        Ovl11ActivateEntityAtPosition(3, -200, 992, -1081);
        Ovl11ActivateEntityAtPosition(4, -200, 992, -630);
        return 1;
    }
    phase = *(s16 *)&gOvl11ValueSlots[phaseSlot];
    result = 0;
    switch (phase) {
    case 0:
        /* Admission reports separately; it does not advance the phase. */
        item = (s16 *)&Ovl11Item_80071A8A;
        if (Ovl11SelectionCatalogContainsId(item[0]) == 1) {
            result = 1;
            gOvl11ValueSlots[itemValueSlot] = item[1];
        } else if (item[0] == 0x87) {
            gOvl11ValueSlots[itemValueSlot] = 4;
            result = 1;
        }
        if (result == 1) {
            ovl_11_func_800F0A58(item[0]);
            D_80070CB4 = 1000;
            gOvl11ValueSlots[itemIdSlot] = item[0];
            Ovl11MatchItemSlotsCommand(item[0], 1, 0, 1);
        }
        return result;
    case 1:
        scoreRow = ((Ovl11SceneState *)D_8006C838)->level;
        Ovl11WriteStateRecordPosition(0x1E, 0, -0x4CB, 0x3F2);
        Ovl11StateRecordAnimationCommand(0x1E, 0, 2, 1);
        Ovl11WriteStateRecordPosition(0x16, 0, -0x524, 0x6FE);
        Ovl11StateRecordAnimationCommand(0x16, 0, 0, 1);
        Ovl11WriteStateRecordPosition(3, 0, -0x31D, 0x6FE);
        Ovl11StateRecordAnimationCommand(3, 0, 0, 1);
        Ovl11WriteStateRecordPosition(0xD, 0, -0x573, -0x3C9);
        Ovl11StateRecordAnimationCommand(0xD, 0, 3, 1);
        Ovl11WriteStateRecordPosition(6, 0, 0x57B, -0x54D);
        Ovl11StateRecordAnimationCommand(6, 0, 1, 1);
        Ovl11WriteStateRecordPosition(7, 0, 0x57B, -0x338);
        Ovl11StateRecordAnimationCommand(7, 0, 1, 1);
        Ovl11WriteStateRecordPosition(0xC, 0, -0x8D0, -0x6A6);
        Ovl11StateRecordAnimationCommand(0xC, 0, 3, 1);
        Ovl11WriteStateRecordPosition(0xE, 0, -0x855, -0x8E4);
        Ovl11StateRecordAnimationCommand(0xE, 0, 3, 1);
        Ovl11WriteStateRecordPosition(5, 0, -0x6BB, -0xA55);
        Ovl11StateRecordAnimationCommand(5, 0, 2, 1);
        Ovl11WriteStateRecordPosition(8, 0, 0x333, -0xA01);
        Ovl11StateRecordAnimationCommand(8, 0, 2, 1);
        Ovl11WriteStateRecordPosition(9, 0, 0x70B, -0xA01);
        Ovl11StateRecordAnimationCommand(9, 0, 2, 1);
        Ovl11WriteStateRecordPosition(0x18, 0, -0x9A3, 0x5BC);
        Ovl11StateRecordAnimationCommand(0x18, 0, 3, 1);
        Ovl11WriteStateRecordPosition(0xA, 0, -0x928, 0x809);
        Ovl11StateRecordAnimationCommand(0xA, 0, 3, 1);
        Ovl11WriteStateRecordPosition(0x11, 0, 0x541, 0x8C9);
        Ovl11StateRecordAnimationCommand(0x11, 0, 0, 1);
        Ovl11WriteStateRecordPosition(2, 0, -0x843, 0x327);
        Ovl11StateRecordAnimationCommand(2, 0, 3, 1);
        if (ovl_11_func_800EDF3C(0x14, 0, 0, 0) == 1) {
            Ovl11WriteStateRecordPosition(0x14, 0, 0x51F, -0xA01);
            Ovl11StateRecordAnimationCommand(0x14, 0, 2, 1);
        }

        best = 0;
        if (scoreRow >= 5) { scoreRow = 4; }
        for (i = 0; i < 5; i++) {
            scores.values[scoreRow][i] += Rand(gOvl11SceneSequenceRandomRanges[scoreRow][i]);
            if (scores.values[scoreRow][best] < scores.values[scoreRow][i]) {
                best = i;
            }
        }
        if (gOvl11ValueSlots[itemIdSlot] != 0) {
            Ovl11ApplyWorkPositionCommand(0, 0, 1001, 1790);
            Ovl11SelectObjectStateOrDirectionCommand(0, 0, 0, 0);
            ((Ovl11SceneState *)D_8006C838)->y = -400;
            ((Ovl11SceneState *)D_8006C838)->x = 887;
            ((Ovl11SceneState *)D_8006C838)->z = 1352;
            if (scores.values[scoreRow][best] <= gOvl11ValueSlots[itemValueSlot]) {
                best = -1;
                Ovl11BroadcastStateFieldCommand(32, 1, 20, 2000);
            }
        }
        /* Candidate index 1 deliberately leaves the old selection intact. */
        switch ((s16)(best + 1)) {
        case 0: gOvl11ValueSlots[selectedRecordSlot] = 41; break;
        case 1: gOvl11ValueSlots[selectedRecordSlot] = 22; break;
        case 3: gOvl11ValueSlots[selectedRecordSlot] = 13; break;
        case 4: gOvl11ValueSlots[selectedRecordSlot] = 6; break;
        case 5: gOvl11ValueSlots[selectedRecordSlot] = 7; break;
        }
        Ovl11SelectCameraPositionRecord(30, 0, 0, 0);
        result = 1;
        break;
    case 2:
        result = Ovl11RequestSceneSoundBank(0x13, 1, 0, 0);
        break;
    case 3:
        result = Ovl11ScreenFadeCommand(1, 0, 0, 0);
        if (result == 1) {
            Ovl11StartSceneSoundSequence(0, 0, 0, 0);
        }
        break;
    case 4:
        Ovl11StateRecordAnimationCommand(0x1E, 7, 0, 0);
        result = Ovl11StepMessageCommand(1, 0x45C, -1, 0);
        break;
    case 5:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 3, -0x28D, 4);
        break;
    case 6:
        Ovl11StateRecordAnimationCommand(0x1E, 7, 0, 0);
        result = Ovl11StepMessageCommand(1, 0x45E, -1, 0);
        break;
    case 7:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 0, -0x3E7, 1);
        break;
    case 8:
        Ovl11StateRecordAnimationCommand(0x1E, 6, 0, 0);
        result = Ovl11StepMessageCommand(1, 0x45D, -1, 0);
        break;
    case 9:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 0, -0x439, 1);
        break;
    case 10:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 3, 0x26A, 4);
        break;
    case 11:
        Ovl11StateRecordAnimationCommand(0x1E, 8, 0, 0);
        result = Ovl11StepMessageCommand(1, 0x45F, -1, 0);
        break;
    case 12:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 2, -0x276, 3);
        break;
    case 13:
        Ovl11StateRecordAnimationCommand(0x1E, 8, 0, 0);
        result = Ovl11StepMessageCommand(1, 0x460, -1, 0);
        break;
    case 14:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 2, 0x3C9, 3);
        break;
    case 15:
        result = Ovl11AnimateStateRecordMotion12(0x1E, 3, 0x3DB, 4);
        break;
    case 16:
        Ovl11StateRecordAnimationCommand(0x1E, 7, 0, 0);
        if (gOvl11ValueSlots[selectedRecordSlot] == 0x29) {
            result = Ovl11StepMessageCommand(1, 0x461, -1, 0);
        } else {
            result = Ovl11StepMessageCommand(1, 0x462, -1, 0);
        }
        break;
    case 17:
        Ovl11StateRecordAnimationCommand(0x1E, 0, 2, 1);
        Ovl11MasterVolumeRampCommand(0, 0x3C, 1, 0);
        Ovl11ScreenFadeCommand(0, 0x3C, 1, 0);
        result = 1;
        break;
    case 18:
        result = Ovl11ScreenFadeCommand(0, 0, 0, 0);
        break;
    case 19:
        Ovl11ClockAdvanceCommand(0xF, 0, 0, 0);
        Ovl11WriteStateRecordPosition(0x1E, 0, 0x4E, 0x8CF);
        Ovl11StateRecordAnimationCommand(0x1E, 0, 0, 1);
        Ovl11WriteStateRecordPosition(0x11, 0, -0x14C, 0x8CF);
        Ovl11StateRecordAnimationCommand(0x11, 0, 0, 1);
        if (gOvl11ValueSlots[itemIdSlot] != 0) {
            Ovl11ApplyWorkPositionCommand(0, 0, -0x7F, 0xCD);
        } else {
            Ovl11ApplyWorkPositionCommand(0, 0, 0x1E8, -0x35D);
        }
        Ovl11SelectObjectStateOrDirectionCommand(2, 0, 0, 0);
        Ovl11WriteStateRecordPosition(0x16, 0, -0x30F, 0x2E2);
        Ovl11StateRecordAnimationCommand(0x16, 0, 2, 1);
        Ovl11WriteStateRecordPosition(3, 0, 0x211, 0xCD);
        Ovl11StateRecordAnimationCommand(3, 0, 2, 1);
        switch (gOvl11ValueSlots[selectedRecordSlot]) {
        case 0x29: x = -0x7F; z = 0xCD; break;
        case 0x16: x = -0x30F; z = 0x2E2; break;
        case 6: x = -0x30F; z = 0xCD; break;
        case 7: x = 0x211; z = 0x2E2; break;
        default: x = -0x7F; z = 0x2E2; break;
        }
        Ovl11WriteStateRecordPosition(0xD, 0, x, z);
        Ovl11StateRecordAnimationCommand(0xD, 0, 2, 1);
        Ovl11WriteStateRecordPosition(6, 0, -0x30F, 0xCD);
        Ovl11StateRecordAnimationCommand(6, 0, 2, 1);
        Ovl11WriteStateRecordPosition(7, 0, 0x211, 0x2E2);
        Ovl11StateRecordAnimationCommand(7, 0, 2, 1);
        if (gOvl11ValueSlots[selectedRecordSlot] == 0x29) {
            Ovl11ApplyWorkPositionCommand(0, 0, -0x7F, 0x2E2);
        } else {
            Ovl11WriteStateRecordPosition((s16)gOvl11ValueSlots[selectedRecordSlot], 0, -0x7F, 0x2E2);
        }
        Ovl11MoveCameraTarget(-1, 0, 0x29, 0x47C);
        result = Ovl11RequestSceneSoundBank(0xF, 0, 0, 0);
        break;
    case 20:
        Ovl11MasterVolumeRampCommand(1, 0x3C, 1, 0);
        Ovl11ScreenFadeCommand(1, 0x3C, 1, 0);
        result = 1;
        break;
    case 21:
        result = Ovl11ScreenFadeCommand(1, 0, 0, 0);
        break;
    case 22:
        Ovl11CdAudioCommand(3, 0, 1, 0);
        result = 1;
        break;
    case 23:
        result = Ovl11CdAudioCommand(3, 0, 0, 0);
        if (result == 1) {
            Ovl11StartSceneSoundSequence(0, 0, 0, 0);
        }
        break;
    case 24:
        switch (gOvl11ValueSlots[selectedRecordSlot]) {
        case 22: message = 0x474; break;
        case 13: message = 0x476; break;
        case 6: message = 0x478; break;
        case 7: message = 0x47A; break;
        case 41:
        default: message = 0x472; break;
        }
        result = Ovl11StepMessageCommand(1, message, -1, 0);
        break;
    case 25:
        if (gOvl11ValueSlots[selectedRecordSlot] == 0x29) {
            result = Ovl11UpdateWorkPositionBound(0, 2, 0x5A4, 0);
        } else {
            result = Ovl11AnimateStateRecordMotion12((s16)gOvl11ValueSlots[selectedRecordSlot], 2, 0x5A4, 3);
        }
        break;
    case 26:
        if (gOvl11ValueSlots[selectedRecordSlot] == 0x29) {
            Ovl11SelectObjectStateOrDirectionCommand(2, 0, 0, 0);
        } else {
            Ovl11StateRecordAnimationCommand((s16)gOvl11ValueSlots[selectedRecordSlot], 0, 2, 1);
        }
        switch (gOvl11ValueSlots[selectedRecordSlot]) {
        case 22: message = 0x475; break;
        case 13: message = 0x477; break;
        case 6: message = 0x479; break;
        case 7: message = 0x47B; break;
        case 41:
        default: message = 0x473; break;
        }
        result = Ovl11StepMessageCommand(1, message, -1, 0);
        break;
    case 27:
        result = Ovl11SelectCameraPositionRecord(-2, 0, 1, 2);
        break;
    }
    if (result == 1) {
        /* Without an admitted item, skip phases 14–16 after phase 13. */
        if (phase != 13 || (nextPhase = 17, gOvl11ValueSlots[itemIdSlot] != 0)) {
            nextPhase = (s16)(phase + 1);
        }
        phase = nextPhase;
    }
    gOvl11ValueSlots[phaseSlot] = phase;
    if (phase != 20 && phase < 28) {
        result = 0;
    }
    return result;
}
