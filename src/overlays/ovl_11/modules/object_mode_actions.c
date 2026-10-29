/*
 * Object-mode frame callbacks and action execution. The resident dispatcher
 * selects modes 0..12 through gOvl11ObjectModeHandlers (slot 13 is null):
 * 0..5 keyframes, 6 choice/message, 7 movement, 8/9 grid actions, 10 routing,
 * 11 action selection and 12 placement/return. Return zero retains control;
 * one releases it to dispatcher selection. Objects and scene resources must
 * already be initialized; these updates mutate movement, animation and work.
 *
 * The five action programs are helpers of these callbacks, not another public
 * dispatch bank. Their movement step calls mode 7 in turn. Object +28 holds
 * sequence state, +B6 the action ID, +BA selection and +DC completion state;
 * concurrent sequences must not share those fields on one object.
 * Keyframe callbacks arm lock 0x800 and release it on the update AFTER state
 * reaches -1; modes 0/1/5 then set +2C to 300 update ticks. Timers count calls,
 * not milliseconds. The nested mode-10 resolver retains its GCC static chain.
 * Its callback contract is in ovl11_object_mode_transition.h. Admission and
 * the general grid/record services remain external; no linkage is narrowed.
 * Membership and evidence-limited -G0 boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_keyframe_step.h"
#include "sound_commands.h"
#include "game_flags.h"
#include "ovl11_collision_query_budget.h"
#include "ovl11_cell_actions.h"
#include "ovl11_grid_position.h"
#include "ovl11_direction_vector.h"
#include "ovl11_area_slot_flags.h"
#include "ovl11_save_counters.h"
#include "ovl11_record_position_tables.h"
#include "psyq/stddef.h"
#include "ovl11_tagged_record_handlers.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "message_window.h"
#include "ovl11_cell_geometry.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_object_mode_transition.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_seasonal_record_ids.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_eligible_record_selection.h"
#include "ovl11_object_record_state.h"
#include "ovl11_record_position_set.h"
#include "ovl11_item_value_counters.h"

/* These cross-bank helper calls retain their original external symbols. */
s32 Ovl11StepGridTargetAction(Ovl11SetFieldsView *object, s32 gridMode);
s32 Ovl11StepGridTargetActionAlternate(Ovl11SetFieldsView *object, s32 gridMode);
s32 Ovl11StepRecordSelectionAction(Ovl11StateArg0 *object, s32 mode);
s32 Ovl11StepTaggedRecordAction(Ovl11SetFieldsView *object, s32 unused);
s32 Ovl11StepSeasonalRecordAction(Ovl11SetFieldsView *object, s32 unused);
s32 Ovl11LookupActionPosition(Ovl11StateArg0 *object, s16 mode,
                            s32 *position, s16 key);
s32 Ovl11AdvanceActionMovement(Ovl11E644Object *object, s16 threshold);

s32 Ovl11StepObjectMode0Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode0Keyframes, &object->unk28, -1, (u16 *) &object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode1Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode1Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode2Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode2Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode3Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode3Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode4Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode4Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode7Movement(Ovl11E644Object *object) {
    s16 idleAnimation;
    s16 movingAnimation;

    Ovl11StepRecordMovement((Recon_ovl_11_func_800D2594_A0View *) object, Ovl11GetRecordMovePeriod((s32) object), 0x70);
    idleAnimation = 0xF;
    if (object->unkB6 == 0) {
        idleAnimation = 0;
        movingAnimation = 1;
    } else {
        movingAnimation = 6;
    }
    if ((object->unk48 == 0) && (object->unk4C == 0) && (object->unk50 == 0)) {
        object->unk24 = idleAnimation;
    } else {
        object->unk24 = movingAnimation;
    }
    object->unk34 &= ~0x800;
    return 1;
}

s32 Ovl11RunObjectMode6ChoiceMessage(Ovl11E6E8Object *obj) {
    s16 x;
    s16 y;
    s32 result;
    s32 facing;
    u16 direction;
    s32 first;
    s32 second;
    s32 message;
    s32 number;

    result = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_51EE = 1;
    ((Ovl11E6E8State *) D_8006C838)->field_522C = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_5230 = 0;
    ((Ovl11E6E8State *) D_8006C838)->field_5234 |= 0x800400;
    direction = ((Ovl11E6E8State *) D_8006C838)->field_5200;
    if (direction < 2U) {
        facing = direction;
        facing += 2;
    } else {
        facing = direction;
        facing -= 2;
    }
    obj->direction = facing;
    switch (obj->state) {
    case 0:
        obj->flags |= 0x800;
        switch (GetMessageWindowChoice()) {
        case 0:
            break;
        case 1:
            obj->state = 2;
            if (Ovl11FindCandidateGridCell(&x, &y, 1, 1, 1) != 0 &&
                Ovl11FindCandidateGridCell(&x, &y, 0, 1, 1) != 0) {
                obj->state = 4;
            }
            obj->attributes |= 0x41;
            break;
        case 2:
            obj->state = 2;
            if (Ovl11FindCandidateGridCell(&x, &y, 1, 0, 0) != 0 &&
                Ovl11FindCandidateGridCell(&x, &y, 0, 0, 0) != 0) {
                obj->state = 4;
            }
            obj->attributes |= 0x42;
            break;
        case 3:
            obj->state = 2;
            first = Ovl11ChooseEligibleRecord(1, 0xF);
            if (first == -1) {
                second = Ovl11ChooseEligibleRecord(4, 0xF);
                if (second == first && Ovl11ChooseEligibleRecord(5, 0xF) == second) {
                    obj->state = 4;
                }
            }
            obj->attributes |= 0x44;
            break;
        case 4:
            obj->state = 5;
            break;
        }
        break;
    case 2:
        message = gOvl11Mode6MessageIds[obj->index];
        number = message + 3;
        if (obj->index != 0) {
            number = message + 2;
        }
        RequestMessageWindow(2, number);
        obj->state = 3;
        break;
    case 3:
        switch (GetMessageWindowChoice()) {
        case 0:
            break;
        case 1:
            obj->choice = 2;
            obj->state = -1;
            RequestMessageWindow(2, obj->index + 0x26C);
            break;
        case 2:
            obj->choice = 4;
            obj->state = -1;
            RequestMessageWindow(2, obj->index + 0x26C);
            break;
        case 3:
            obj->choice = 8;
            obj->state = -1;
            RequestMessageWindow(2, obj->index + 0x26C);
            break;
        case 4:
            obj->state = 5;
            break;
        }
        break;
    case 4:
        RequestMessageWindow(2, 0x149);
        obj->state = 1;
        break;
    case 5:
        message = gOvl11Mode6MessageIds[obj->index];
        number = message + 4;
        if (obj->index != 0) {
            number = message + 3;
        }
        RequestMessageWindow(2, number);
        obj->state = 1;
        break;
    case 1:
        obj->state = -1;
        obj->attributes &= ~0x47;
        /* fall through */
    case -1:
        ((Ovl11E6E8State *) D_8006C838)->field_51EE = 0;
        ((Ovl11E6E8State *) D_8006C838)->field_5234 &= ~0x400;
        ((Ovl11E6E8State *) D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        obj->timer = 0x12C;
        obj->flags &= ~0x800;
        break;
    }
    return result;
}

s32 Ovl11StepObjectMode5Keyframes(Ovl11D04D4Obj *object) {
    s16 state;
    s32 released;

    state = object->unk28;
    released = 0;
    switch (state) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        released = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11Mode5Keyframes, &object->unk28, -1, (u16 *) &object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return released;
}

s32 Ovl11StepObjectMode8GridAction(Ovl11E644Object *arg0) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 sp2A;
    s16 temp_v1;
    s32 temp_a0;
    s32 var_s1_2;

    var_s1_2 = 1;
    temp_a0 = arg0->unk34 | 0x800;
    arg0->unk34 = temp_a0;
    if ((*(s16 *) ((u8 *) arg0 + 0x28)) == -1) {
        Ovl11AdvanceActionMovement(arg0, 0xE6);
        if ((*(s16 *) ((u8 *) arg0 + 0x30)) == 6) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
    } else {
        temp_v1 = *(s16 *) ((u8 *) arg0 + 0x30);
        if (temp_v1 == 1) {
            arg0->unk34 = temp_a0 & 0xC3FFFFFF;
            if (Ovl11StepGridTargetAction((Ovl11SetFieldsView *) arg0, 1) != 0) {
                if (Ovl11FindCandidateGridCell(&sp28, &sp2A, 0, 1, 1) == 0) {
                    Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 3, &sp18.vx, *(s16 *) ((u8 *) arg0 + 0x30));
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, sp18);
                    (*(s16 *) ((u8 *) arg0 + 0x28)) = -1;
                    arg0->unk34 |= 0x10000000;
                } else {
                    goto block_9;
                }
            }
        } else if ((temp_v1 != 6) || (arg0->unk34 = temp_a0 & 0xC3FFFFFF, (Ovl11StepGridTargetAction((Ovl11SetFieldsView *) arg0, 0) != 0))) {
block_9:
            var_s1_2 = 0;
        }
    }
    if (var_s1_2 == 1) {
        var_s1_2 = 0;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x26));
        (*(u16 *) ((u8 *) arg0 + 0xC0)) = (u16) (*(s16 *) ((u8 *) arg0 + 0x28));
        (*(u16 *) ((u8 *) arg0 + 0xC2)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x2A));
    } else {
        var_s1_2 = 1;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = 0U;
        arg0->unk34 &= ~0x800;
        (*(u16 *) ((u8 *) arg0 + 0xB8)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xB8)) | 0x40);
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 7);
        arg0->unk34 |= 0x3C000000;
    }
    return var_s1_2;
}

s32 Ovl11StepObjectMode9GridAction(Ovl11E644Object *arg0) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 sp2A;
    s16 temp_v1;
    s32 temp_a0;
    s32 var_s1_2;

    var_s1_2 = 1;
    temp_a0 = arg0->unk34 | 0x800;
    arg0->unk34 = temp_a0;
    if ((*(s16 *) ((u8 *) arg0 + 0x28)) == -1) {
        Ovl11AdvanceActionMovement(arg0, 0xE6);
        if ((*(s16 *) ((u8 *) arg0 + 0x30)) == 6) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
    } else {
        temp_v1 = *(s16 *) ((u8 *) arg0 + 0x30);
        if (temp_v1 == 1) {
            arg0->unk34 = temp_a0 & 0xC3FFFFFF;
            if (Ovl11StepGridTargetActionAlternate((Ovl11SetFieldsView *) arg0, 1) != 0) {
                if (Ovl11FindCandidateGridCell(&sp28, &sp2A, 0, 0, 1) == 0) {
                    Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 3, &sp18.vx, *(s16 *) ((u8 *) arg0 + 0x30));
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, sp18);
                    (*(s16 *) ((u8 *) arg0 + 0x28)) = -1;
                    arg0->unk34 |= 0x10000000;
                } else {
                    goto block_9;
                }
            }
        } else if ((temp_v1 != 6) || (arg0->unk34 = temp_a0 & 0xC3FFFFFF, (Ovl11StepGridTargetActionAlternate((Ovl11SetFieldsView *) arg0, 0) != 0))) {
block_9:
            var_s1_2 = 0;
        }
    }
    if (var_s1_2 == 1) {
        var_s1_2 = 0;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x26));
        (*(u16 *) ((u8 *) arg0 + 0xC0)) = (u16) (*(s16 *) ((u8 *) arg0 + 0x28));
        (*(u16 *) ((u8 *) arg0 + 0xC2)) = (u16) (*(u16 *) ((u8 *) arg0 + 0x2A));
    } else {
        var_s1_2 = 1;
        (*(u16 *) ((u8 *) arg0 + 0xBE)) = 0U;
        arg0->unk34 &= ~0x800;
        (*(u16 *) ((u8 *) arg0 + 0xB8)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xB8)) | 0x40);
        Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 7);
        arg0->unk34 |= 0x3C000000;
    }
    return var_s1_2;
}

/*
 * Stateful routing between object modes 1, 4 and 5.
 *
 * Public callback contract: ovl11_object_mode_transition.h. The nested
 * resolver captures the enclosing object; the original's static-chain loads
 * prove the two functions share a TU. The enclosing mode bank adds the other
 * frame callbacks; mode admission and action programs remain external.
 * Membership and evidence-limited outer boundaries: notes/file-groupings.md.
 */

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
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
                } else if ((selected = Ovl11ChooseEligibleRecord(5, 6)) != -1) {
                    object->unk28 = -1;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(4, 1) != selected && gSaveCounter20 != 0) {
                    object->unk28 = -2;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(4, 8) != -1) {
                    object->unk28 = -2;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
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
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
                } else if (Ovl11ChooseEligibleRecord(1, 10) != -1) {
                    object->unk28 = -3;
                    transition->flags |= 0x100;
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 5, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
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
                    Ovl11LookupActionPosition((Ovl11StateArg0 *)transition, 4, &position.vx, object->unk30);
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, position);
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

s32 Ovl11StepObjectMode11ActionSelect(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_s3;
    u16 temp_a0;
    Ovl11PaddedVec3 position;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    var_s3 = 0;
    switch (temp_v1) {
    case 0:
        *(s16 *) ((u8 *) arg0 + 0x28) = 1;
        *(s16 *) ((u8 *) arg0 + 0x22) = 0;
        arg0->field_34 |= 0x800;
        position.vx = -0xCE4;
        position.vy = 0;
        position.vz = 0x960;
        Ovl11SetRecordPosition(arg0, position);
        *(s32 *) ((u8 *) arg0 + 0x38) = -0xCE4;
        *(s32 *) ((u8 *) arg0 + 0x3C) = 0;
        arg0->field_30 = 1;
        *(s32 *) ((u8 *) arg0 + 0x40) = 0x16A8 - (*(u16 *) ((u8 *) arg0 + 0xAC) * 0xC8);
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0xE6) == 0) {
            break;
        }
        *(s16 *) ((u8 *) arg0 + 0x28) = 2;
        /* fall through */
    case 2:
        temp_a0 = *(u16 *) ((u8 *) arg0 + 0xB8);
        arg0->field_34 &= ~0x800;
        if (temp_a0 & 1) {
            Ovl11StepObjectMode7Movement((Ovl11E644Object *) arg0);
            Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 8);
        } else if (temp_a0 & 2) {
            Ovl11StepObjectMode7Movement((Ovl11E644Object *) arg0);
            Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 9);
        } else if (temp_a0 & 4) {
            *(s16 *) ((u8 *) arg0 + 0x22) = 3;
            Ovl11StepObjectMode7Movement((Ovl11E644Object *) arg0);
            Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 0xA);
        } else {
            Ovl11StepObjectMode7Movement((Ovl11E644Object *) arg0);
            Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 7);
        }
        var_s3 = 1;
        break;
    }
    return var_s3;
}

s32 Ovl11StepObjectMode12PlacementReturn(Ovl11E644Object *arg0) {
    Ovl11PaddedVec3 position;
    s32 result;

    result = 0;
    arg0->unk34 |= 0x3C000800;
    if (*(s16 *) ((u8 *) arg0 + 0x28) == -1) {
        Ovl11AdvanceActionMovement(arg0, 0xE6);
        if (*(s16 *) ((u8 *) arg0 + 0x30) == 1) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
        }
    } else {
        if (*(s16 *) ((u8 *) arg0 + 0x30) == 1) {
            switch (*(s16 *) ((u8 *) arg0 + 0x28)) {
            case 0:
                arg0->unk34 |= 0x800;
                *(s16 *) ((u8 *) arg0 + 0x28) = 1;
                position.vx = -0xCE4;
                position.vy = 0;
                position.vz = 0xFA0;
                Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, position);
                break;
            case 1:
                if (!Ovl11AdvanceActionMovement(arg0, 0x258)) {
                    break;
                }
                *(s16 *) ((u8 *) arg0 + 0x28) = 2;
                /* fall through */
            case 2:
                arg0->unk24 = 1;
                *(s16 *) ((u8 *) arg0 + 0x22) = 2;
                Ovl11SetDirectionVector(2, (Recon800D0408A1View *) &arg0->unk48, 0x18);
                if (*(s32 *) ((u8 *) arg0 + 0x40) >= 0x1A2D) {
                    Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *) arg0);
                    arg0->unk34 &= ~0x800;
                    Ovl11SetObjectRecordMode((Recon_ovl_11_func_8010CE80_A0View *) arg0, 0);
                    result = 1;
                }
                break;
            }
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 4) {
            Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 4, &position.vx, 4);
            Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 5) {
            Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 5, &position.vx, 5);
            Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 6) {
            Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 3, &position.vx, 6);
            Ovl11SetRecordPosition((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        }
    }
    return result;
}

/* The action programs may move objects, consume/select records and change
 * sprites, flags and sound. Grid variants return one for acquisition failure,
 * otherwise zero even on normal completion (which resets +28). gridMode 1
 * selects the mode-1 search; the alternate promotes a pending cell instead
 * of performing the full completion sequence. The record program returns one
 * on resource/selection failure or final release; tagged/seasonal programs
 * return one only for initial selection failure. Their second word is unread.
 * Do not interpret these different completion conventions as one Boolean API.
 * Record mode 1 selects counter-18/Z600 placement, otherwise counter-20/X400.
 * Tagged and seasonal programs use selector modes 6 and 8 respectively. */

/* Internal helpers retain their original external linkage. */
s32 Ovl11AcquireActionGridTarget(Ovl11SetFieldsView *object, s32 gridMode, s32 option);
s32 Ovl11ProbeOffsetActionKey(Ovl11ActionProbeObject *object);
s32 Ovl11TestActionKey5(s32 objectAddress, s16 unusedActionId);
s32 Ovl11TestActionKeyCounter(Ovl11StateKeyProbeView *object, s16 key, s16 threshold);
void Ovl11PromoteOffsetGridCell(Ovl11StateDirectionView *object, s32 gridMode);
u16 Ovl11CompleteActionSequence(Ovl11ActionCompletionView *object);

s32 Ovl11StepGridTargetAction(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s32 temp_v0;
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        if (Ovl11AcquireActionGridTarget(arg0, arg1, 1) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
            return 1;
        }
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 1;
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0xE6) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        }
        break;
    case 2:
        temp_v0 = Ovl11ProbeOffsetActionKey((Ovl11ActionProbeObject *) arg0);
        (*(s16 *) ((u8 *) arg0 + 0xB6)) = temp_v0;
        if (temp_v0 & 0xFFFF) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    case 3:
        if (Ovl11TestActionKey5((s32) arg0, *(s16 *) ((u8 *) arg0 + 0xB6)) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 4;
        }
        break;
    case 4:
        if (Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 2, (s32 *) &sp18, arg0->field_30) == 1) {
            Ovl11SetRecordPosition(arg0, sp18);
        }
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 5;
        break;
    case 5:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0x258) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 6;
        }
        break;
    case 6:
        if (Ovl11CompleteActionSequence((Ovl11ActionCompletionView *) arg0) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    }
    return 0;
}

s32 Ovl11StepGridTargetActionAlternate(Ovl11SetFieldsView *arg0, s32 arg1) {
    switch (*(s16 *) ((u8 *) arg0 + 0x28)) {
    case 0:
        if (Ovl11AcquireActionGridTarget(arg0, arg1, 0) != 0) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
            return 1;
        }
        *(s16 *) ((u8 *) arg0 + 0x28) = 1;
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0xE6) != 0) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 2;
        }
        break;
    case 2:
        if (Ovl11TestActionKeyCounter((Ovl11StateKeyProbeView *) arg0, 8, 0) != 0) {
            Ovl11PromoteOffsetGridCell((Ovl11StateDirectionView *) arg0, arg1);
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
        }
        break;
    }
    return 0;
}

s32 Ovl11StepRecordSelectionAction(Ovl11StateArg0 *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s32 selected;
    s32 result;

    switch (*(s16 *)((u8 *)arg0 + 0x28)) {
    case 0:
        if (Ovl11ChooseEligibleRecord(*(s16 *)((u8 *)arg0 + 0x30), 1) == -1) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
            return 1;
        }
        Ovl11LookupActionPosition(arg0, arg1 == 0, &sp18.vx, *(s16 *)((u8 *)arg0 + 0x30));
        Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0, sp18);
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        *(u16 *)((u8 *)arg0 + 0xB8) |= 0x200;
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        }
        break;
    case 2:
        if (arg1 == 1) {
            if (Ovl11TakeSaveCounter18(1)) {
                *(s16 *)((u8 *)arg0 + 0xB6) = 0x68;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                return 1;
            }
        } else {
            if (Ovl11TakeSaveCounter20(1)) {
                *(s16 *)((u8 *)arg0 + 0xB6) = 0x6B;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                return 1;
            }
        }
        *(s16 *)((u8 *)arg0 + 0x28) = 3;
        /* fallthrough */
    case 3:
        if (Ovl11TestActionKey5((s32)arg0, *(s16 *)((u8 *)arg0 + 0xB6))) {
            selected = Ovl11ChooseEligibleRecord(*(s16 *)((u8 *)arg0 + 0x30), 1);
            if (selected != -1) {
                *(s16 *)((u8 *)arg0 + 0xBA) = selected;
                if (arg1 == 1) {
                    Ovl11GetSlotPositionZ600(&sp18.vx, selected);
                } else {
                    Ovl11GetSlotPositionX400(&sp18.vx, selected);
                }
                Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0, sp18);
                *(s16 *)((u8 *)arg0 + 0x28) = 4;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 6;
            }
        }
        break;
    case 4:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 5;
        }
        break;
    case 5:
        result = Ovl11IsAreaSlotFlagSet(*(s16 *)((u8 *)arg0 + 0xBA), arg1);
        if (result == 1) {
            selected = Ovl11ChooseEligibleRecord(*(s16 *)((u8 *)arg0 + 0x30), 1);
            if (selected == -1) {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
            } else {
                *(s16 *)((u8 *)arg0 + 0xBA) = selected;
                if (arg1 == 1) {
                    Ovl11GetSlotPositionZ600(&sp18.vx, selected);
                } else {
                    Ovl11GetSlotPositionX400(&sp18.vx, selected);
                }
                Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0, sp18);
                *(s16 *)((u8 *)arg0 + 0x28) = 4;
            }
        } else {
            Ovl11SetAreaSlotFlag(*(s16 *)((u8 *)arg0 + 0xBA), arg1);
            Ovl11ClearRecordItemSprite((u8 *)arg0);
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
        }
        break;
    case 7:
        if (arg1 == 1) {
            Ovl11LookupActionPosition(arg0, 0, &sp18.vx, *(s16 *)((u8 *)arg0 + 0x30));
        } else {
            Ovl11LookupActionPosition(arg0, 1, &sp18.vx, *(s16 *)((u8 *)arg0 + 0x30));
        }
        Ovl11SetRecordPosition((Ovl11SetFieldsView *)arg0, sp18);
        *(s16 *)((u8 *)arg0 + 0x28) = 8;
        /* fallthrough */
    case 8:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0xE6)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 6;
        }
        break;
    case 6:
        if (arg1 == 1) {
            Ovl11AddSaveCounter18(0, 1);
        } else {
            Ovl11AddSaveCounter20(0, 1);
        }
        Ovl11ClearRecordItemSprite((u8 *)arg0);
        *(s16 *)((u8 *)arg0 + 0x28) = 0;
        return 1;
    }
    return 0;
}

s32 Ovl11StepTaggedRecordAction(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 position;
    Ovl11ActionDispatchView *record;
    Ovl11ActionDispatchView *animal;
    u8 *base;
    s16 index;
    s32 selected;
    s32 state;

    state = -1;
    index = *(s16 *)((u8 *)arg0 + 0xBA);
    if (index != state) {
        animal = (Ovl11ActionDispatchView *)((u8 *)&gTaggedRecordBase + index * 0xB8);
        record = animal;
    } else {
        animal = NULL;
        record = NULL;
    }
    state = *(s16 *)((u8 *)arg0 + 0x28);
    switch (state) {
    case 0:
        selected = Ovl11ChooseEligibleRecord(arg0->field_30, 6);
        if (selected == -1) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
            *(s16 *)((u8 *)arg0 + 0xBA) = selected;
            return 1;
        }
        base = (u8 *)D_8006C838;
        *(s16 *)((u8 *)arg0 + 0xBA) = selected;
        Ovl11SetRecordPosition(arg0,
            ((Ovl11D5224LargeView *)(base + (s16)selected * 0xB8))->position);
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0xE6) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        } else {
            if (*(s16 *)((u8 *)record + 0x30) == arg0->field_30) {
                Ovl11SetRecordPosition(arg0, *(Ovl11PaddedVec3 *)((u8 *)record + 0x38));
                break;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
                break;
            }
        }
        /* fall through */
    case 2:
        if (Ovl11TestActionKeyCounter((Ovl11StateKeyProbeView *)arg0, 9, 0x3C) != 0) {
            if (!(record->unk34 & 0x10000)) {
                Ovl11DispatchCode160Handler(animal, 6);
            } else {
                Ovl11DispatchTaggedRecordHandler(animal, 6);
            }
            Ovl11SetRecordPosition(arg0,
                ((Ovl11D5224LargeView *)((u8 *)D_8006C838 + *(s16 *)((u8 *)arg0 + 0xBA) * 0xB8))->position);
            *(s16 *)((u8 *)arg0 + 0x28) = 4;
        }
        break;
    case 4:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0xE6) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 3;
        } else {
            if (*(s16 *)((u8 *)record + 0x30) == arg0->field_30) {
                Ovl11SetRecordPosition(arg0, *(Ovl11PaddedVec3 *)((u8 *)record + 0x38));
                break;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
                break;
            }
        }
        /* fall through */
    case 3:
        if (Ovl11TestActionKeyCounter((Ovl11StateKeyProbeView *)arg0, 0xA, 0x3C) != 0) {
            animal->unkB6 = *(u16 *)((u8 *)arg0 + 0xAC);
            if (!(record->unk34 & 0x10000)) {
                Ovl11DispatchCode160Handler(animal, 7);
            } else {
                Ovl11DispatchTaggedRecordHandler(animal, 8);
            }
            if (*(u16 *)((u8 *)arg0 + 0xB6) != 0) {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                break;
            }
        } else {
            break;
        }
        /* fall through */
    case 7:
        if (Ovl11TestActionKey5((s32)arg0, *(s16 *)((u8 *)arg0 + 0xB6)) != 0) {
            Ovl11LookupActionPosition((Ovl11StateArg0 *)arg0, 2, &position.vx, arg0->field_30);
            Ovl11SetRecordPosition(arg0, position);
            *(s16 *)((u8 *)arg0 + 0x28) = 5;
        }
        break;
    case 5:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *)arg0, 0x258) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 6;
        }
        break;
    case 6:
        if (Ovl11CompleteActionSequence((Ovl11ActionCompletionView *)arg0) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
        }
        break;
    }
    return 0;
}

s32 Ovl11StepSeasonalRecordAction(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 temp_v0;
    s32 temp_v0_2;
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        temp_v0 = Ovl11ChooseEligibleRecord(arg0->field_30, 8);
        (*(s16 *) ((u8 *) arg0 + 0xBA)) = temp_v0;
        if (temp_v0 == -1) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
            return 1;
        }
        temp_v0_2 = Ovl11ReadSpawnRecord(0x123, &sp18.vx, &sp28, temp_v0);
        if ((temp_v0_2 == 1) && (sp28 == arg0->field_30)) {
            Ovl11SetRecordPosition(arg0, sp18);
            (*(s16 *) ((u8 *) arg0 + 0x28)) = temp_v0_2;
        }
        break;
    case 1:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0xE6) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        } else if (!(*(u16 *) (((struct Ovl11FEF8State *) D_8006C838)->records + (*(s16 *) ((u8 *) arg0 + 0xBA)) * 0x18 + 4) & 1)) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    case 2: {
        Ovl11RecordPresentation sp30;

        if (Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *) (((struct Ovl11FEF8State *) D_8006C838)->records + (*(s16 *) ((u8 *) arg0 + 0xBA)) * 0x18), &sp30) == 1) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
            (*(u16 *) ((u8 *) arg0 + 0xB6)) = sp30.id;
        } else {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    }
    case 3:
        if (Ovl11TestActionKey5((s32) arg0, *(s16 *) ((u8 *) arg0 + 0xB6)) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 4;
        }
        break;
    case 4:
        if (Ovl11LookupActionPosition((Ovl11StateArg0 *) arg0, 2, &sp18.vx, arg0->field_30) == 1) {
            Ovl11SetRecordPosition(arg0, sp18);
        }
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 5;
        break;
    case 5:
        if (Ovl11AdvanceActionMovement((Ovl11E644Object *) arg0, 0x258) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 6;
        }
        break;
    case 6:
        if (Ovl11CompleteActionSequence((Ovl11ActionCompletionView *) arg0) != 0) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        }
        break;
    }
    return 0;
}

u16 Ovl11CompleteActionSequence(Ovl11ActionCompletionView *object) {
    char *farBase;

    Ovl11RecordItemValue(object->actionId);
    farBase = (char *)&D_8007AFF0;
    if (*(s16 *)(farBase + 0x25476) == object->area) {
        PlaySoundEffect(0x13);
        SetGameFlag(7, 1);
    }
    object->actionId = 0;
    object->actionState = 0;
    return 1;
}

/* Search mode 0..5 by signed key. A match copies three raw coordinate words
 * and returns one; failure leaves output untouched. The fourth vector word
 * is never written. This refreshes mutable mode-0/mode-2 rows on EVERY call;
 * modes 0/1 additionally select using object X/Z and pool-expansion state. */
s32 Ovl11LookupActionPosition(Ovl11StateArg0 *object, s16 mode, s32 *position, s16 key) {
    Ovl11ActionPositionRow *row;
    s32 rowCount;
    s32 rowIndex;

    rowCount = 0;
    row = 0;
    /* Restore defaults before applying the current pool-layout adjustment. */
    gOvl11ActionPositionsMode0[0].z = 0x92E;
    gOvl11ActionPositionsMode2[2].x = -0xC8;
    gOvl11ActionPositionsMode2[2].z = -0x9C4;
    switch (mode) {
    case 0:
        rowCount = 1;
        row = gOvl11ActionPositionsMode0;
        if (gTaggedRecordPoolExpanded != 0) {
            if (object->unk38 < 0) {
                row = gOvl11ActionPositionsMode0 + 1;
            }
        } else {
            gOvl11ActionPositionsMode0[0].z -= 0x96;
        }
        break;
    case 1:
        rowCount = 1;
        row = gOvl11ActionPositionsMode1;
        if (gB4RecordPoolExpanded != 0 && object->unk40 > 0) {
            row = gOvl11ActionPositionsMode1 + 1;
        }
        break;
    case 2:
        rowCount = 3;
        if (gB4RecordPoolExpanded == 0) {
            gOvl11ActionPositionsMode2[2].x = 0x226;
            gOvl11ActionPositionsMode2[2].z = -0x960;
        }
        row = gOvl11ActionPositionsMode2;
        break;
    case 3:
        rowCount = 2;
        row = gOvl11ActionPositionsMode3;
        break;
    case 4:
        rowCount = 2;
        row = gOvl11ActionPositionsMode4;
        break;
    case 5:
        rowCount = 2;
        row = gOvl11ActionPositionsMode5;
        break;
    }
    rowIndex = 0;
    for (rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        if (key == row->key) {
            position[0] = row->x;
            position[1] = row->y;
            position[2] = row->z;
            return 1;
        }
        row += 1;
    }
    return 0;
}

s32 Ovl11ProbeOffsetActionKey(Ovl11ActionProbeObject *arg0) {
    Recon800D0408A1View sp20;
    void *sp30;
    s32 sp34;
    s32 result;

    Ovl11SetDirectionVector(arg0->unk22, &sp20, 0x64);
    result = 0;
    arg0->unkDC = 0;
    sp34 = 0;
    sp20.unk0 += arg0->unk38;
    sp20.unk8 += arg0->unk40;
    if (Ovl11QueryCollisionUnbudgeted(&sp20.unk0, arg0->collision, 0, 0,
                           &sp30, &sp34, 0, 0x8026) == 0) {
        sp34 = 0;
    }
    if (sp34 & 0x20) {
        result = Ovl11ResolveCellItemState(sp30);
    }
    return result;
}

void Ovl11PromoteOffsetGridCell(Ovl11StateDirectionView *object, s32 gridMode) {
    Recon800D0408A1View position;
    s16 column;
    s16 row;
    s16 mode;
    s32 (*lookup)(s32 *, u16 *, u16 *);

    if (gridMode == 1) {
        mode = 0;
        lookup = Ovl11LookupMode1GridPosition;
    } else {
        mode = 1;
        lookup = Ovl11LookupMode6GridPosition;
    }
    Ovl11SetDirectionVector(object->direction, &position, 0x64);
    position.unk0 += object->x;
    position.unk8 += object->z;
    if (lookup((s32 *)&position, (u16 *)&column, (u16 *)&row) == 0) {
        Ovl11TryPromoteCellPendingKind(mode, column, row, 0);
    }
}

/* A new key resets the counter without advancing it. Once the threshold
 * is reached, the area/status gate can continue to hold the sequence. */
s32 Ovl11TestActionKeyCounter(Ovl11StateKeyProbeView *object, s16 key, s16 threshold) {
    if (object->actionKey == key) {
        if (object->keyUpdates >= threshold) {
            char *farBase = (char *)&D_8007AFF0;

            if (object->area != *(s16 *)(farBase + 0x25476) || (object->statusFlags & 0x300) != 0) {
                return 1;
            }
        } else {
            object->keyUpdates = object->keyUpdates + 1;
        }
    } else {
        object->actionKey = key;
        object->keyUpdates = 0;
    }
    return 0;
}

/* The second argument is passed by the original callers but unread here. */
s32 Ovl11TestActionKey5(s32 objectAddress, s16 unusedActionId) {
    s32 result;
    result = Ovl11TestActionKeyCounter((Ovl11StateKeyProbeView *)objectAddress, 5, 0);
    return ((u32)0) < ((u32)result);
}

/* threshold is a movement-rank value, not a timer. Preserve the caller's
 * lock across mode 7; once motion ends, clear the direction vector too. */
s32 Ovl11AdvanceActionMovement(Ovl11E644Object *object, s16 threshold) {
    s32 savedFlags;

    savedFlags = object->unk34;
    Ovl11SetMovementRankThreshold(threshold);
    Ovl11StepObjectMode7Movement(object);
    if (savedFlags & 0x800) {
        object->unk34 |= 0x800;
    }
    if (!(object->unk34 & 0x2000)) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) &object->unk48, 0);
        return 1;
    }
    return 0;
}

s32 Ovl11AcquireActionGridTarget(Ovl11SetFieldsView *object, s32 gridMode, s32 option) {
    Ovl11PaddedVec3 position;
    s16 column;
    s16 row;
    s32 coordinateMode;

    coordinateMode = gridMode != 1;
    if (Ovl11FindCandidateGridCell(&column, &row, gridMode, option, 1) != 0) {
        return 1;
    }
    Ovl11GetCellCornerPosition(column, row, (Vec3 *) &position, coordinateMode);
    Ovl11SetRecordPosition(object, position);
    return 0;
}
