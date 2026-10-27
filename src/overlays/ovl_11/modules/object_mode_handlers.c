/*
 * object_mode_handlers — reconstructed translation unit.
 *
 * Complete object-mode frame callback bank (modes 0 through 12) and its
 * nested routing helper, with private keyframe/message tables.
 *
 * The dispatcher selects these callbacks through gOvl11ObjectModeHandlers;
 * the direct movement entry is also used by action submission. Admission
 * guards and action programs remain separate services. The nested resolver
 * retains its real GCC static chain, not a captured-register substitute.
 * Membership and conservative -G0 boundaries: notes/file-groupings.md.
 */

#include "ovl11_object_mode_handlers.h"
#include "game_types.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_direction_vector.h"
#include "ovl11_record_position_tables.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "message_window.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_object_mode_transition.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"
#include "ovl11_eligible_record_selection.h"
#include "ovl11_action_sequences.h"
#include "ovl11_object_record_state.h"
#include "ovl11_record_position_set.h"

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
