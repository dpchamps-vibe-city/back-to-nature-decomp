/*
 * Stateful routing between object modes 1, 4 and 5.
 *
 * Public callback contract: ovl11_object_mode_transition.h. The nested
 * resolver captures the enclosing object; the original's static-chain loads
 * prove the two functions share a TU. The broader dispatcher and the other
 * routing helpers are external dependencies, not established members.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_mode_transition.h"
#include "psyq/memory.h"
#include "ovl11_eligible_record_selection.h"
#include "ovl11_action_sequences.h"
#include "ovl11_object_record_state.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);

s32 Ovl11UpdateObjectModeTransition(Ovl11ModeTransitionObject *transition) {
    Ovl11PaddedVec3 position;
    s32 active;
    s32 result;
    s32 selected;
    Ovl11D55F8Object *object;

    /* A real nested function, not a file-scope helper: action submission
     * consumes the captured outer argument through GCC's static chain.
     * Original nested entry: ovl_11:0x8010EE5C. */
    void Ovl11ResolveObjectModeTransition(Ovl11SetFieldsView *target, s16 mode, s32 flag) {
        if (target->field_30 == mode) {
            *(s16 *)((u8 *)target + 0xBA) = -1;
            *(s16 *)((u8 *)target + 0x28) = 0;
            target->field_34 &= ~flag;
            *(u16 *)((u8 *)target + 0xB8) |= 0x200;
            return;
        }
        target->field_34 |= flag;
        Ovl11AdvanceActionMovement((Ovl11E644Object *)transition, 0xE6);
    }

    active = 1;
    object = (Ovl11D55F8Object *)transition;
    object->unk34 |= 0x800;
    if (object->unk28 == -1) {
        Ovl11ResolveObjectModeTransition((Ovl11SetFieldsView *)object, 5, 0x04000000);
    } else if (object->unk28 == -2) {
        Ovl11ResolveObjectModeTransition((Ovl11SetFieldsView *)object, 4, 0x20000000);
    } else if (object->unk28 == -3) {
        Ovl11ResolveObjectModeTransition((Ovl11SetFieldsView *)object, 1, 0x04000000);
    } else if (object->unk28 == -4) {
        Ovl11ResolveObjectModeTransition((Ovl11SetFieldsView *)object, 1, 0x20000000);
    } else {
        if (object->unk30 == 1) {
            if (!(*(u16 *)((u8 *)object + 0xB8) & 0x100)) {
                if (Ovl11ChooseEligibleRecord(5, 1) != -1 && gSaveCounter18 != 0) {
                    object->unk28 = -1;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else if ((selected = Ovl11ChooseEligibleRecord(5, 6)) != -1) {
                    object->unk28 = -1;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(4, 1) != selected && gSaveCounter20 != 0) {
                    object->unk28 = -2;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(4, 8) != -1) {
                    object->unk28 = -2;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else {
                    transition->flags |= 0x100;
                }
                transition->index = -1;
                transition->flags |= 0x200;
            } else {
                object->unk34 &= 0xC3FFFFFF;
                if (*(u16 *)((u8 *)object + 0xB8) & 0x200) {
                    if (Ovl11StepTaggedRecordAction((Ovl11SetFieldsView *)object, 0) != 0) {
                        transition->index = -1;
                        transition->flags &= 0xFDFF;
                    }
                } else if (Ovl11StepSeasonalRecordAction((Ovl11SetFieldsView *)object, 0) != 0) {
                    active = 0;
                }
            }
        } else if (object->unk30 == 5) {
            object->unk34 &= 0xC3FFFFFF;
            if (*(u16 *)((u8 *)object + 0xB8) & 0x200) {
                if (Ovl11StepRecordSelectionAction((Ovl11StateArg0 *)object, 1) != 0) {
                    transition->index = -1;
                    transition->flags &= 0xFDFF;
                }
            } else if (Ovl11StepTaggedRecordAction((Ovl11SetFieldsView *)object, 1) != 0) {
                if (Ovl11ChooseEligibleRecord(4, 9) != -1) {
                    object->unk28 = -3;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(1, 10) != -1) {
                    object->unk28 = -3;
                    transition->flags |= 0x100;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else {
                    active = 0;
                }
            }
        } else if (object->unk30 == 4) {
            object->unk34 &= 0xC3FFFFFF;
            if (*(u16 *)((u8 *)object + 0xB8) & 0x200) {
                if (Ovl11StepRecordSelectionAction((Ovl11StateArg0 *)object, 0) != 0) {
                    transition->index = -1;
                    transition->flags &= 0xFDFF;
                }
            } else if (Ovl11StepSeasonalRecordAction((Ovl11SetFieldsView *)object, 0) != 0) {
                if (Ovl11ChooseEligibleRecord(1, 10) != -1) {
                    object->unk28 = -4;
                    transition->flags |= 0x100;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.field_0, object->unk30);
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *)object, position);
                } else {
                    active = 0;
                }
            }
        } else {
            active = 0;
        }
    }
    if (active == 1) {
        result = 0;
        transition->saved26 = object->unk26;
        transition->saved28 = object->unk28;
        transition->saved2A = object->unk2A;
    } else {
        result = 1;
        transition->saved26 = 0;
        object->unk34 &= ~0x800;
        transition->flags |= 0x40;
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *)transition, 7);
        object->unk34 |= 0x3C000000;
        transition->flags &= 0xFCFF;
    }
    return result;
}
