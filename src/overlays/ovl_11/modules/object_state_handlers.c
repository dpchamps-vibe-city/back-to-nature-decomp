/*
 * Object-state entry gates and the complete populated frame-callback bank.
 *
 * The 21-slot dispatch bank lives in object_state_dispatch; this module
 * supplies entry slots 5/6/7 and frame slots 0–4,6–20. Decimal slot numbers
 * name behavior without guessing gameplay roles. Private packed keyframe
 * tables follow member order in the original data.
 *
 * Most sequences arm flag 0x800 on phase 0, advance phase/ticks at +28/+2A,
 * and enter phase -1 on the keyframe terminator. Completion either clears
 * the record or releases 0x800 and sets the outer +2C timer to 300. The
 * dispatcher ignores the callback return value; these effects drive reuse.
 *
 * Directed and plain sequences stay separate functions: each is an original
 * callback identity/table target, not an invitation to merge their bodies.
 * Interfaces: ovl11_object_state_callbacks.h, ovl11_object_state_frames.h.
 * Membership and conservative -G0 boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "game_types.h"
#include "ovl11_object_state_callbacks.h"
#include "ovl11_object_state_frames.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_collision_query_budget.h"
#include "sound_commands.h"
#include "random.h"
#include "ovl11_object_record_clear.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_id_state.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_direction_vector.h"
#include "psyq/memory.h"
#include "ovl11_record_selection.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_small_record_handlers.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *object,
    s32 step, s32 flags);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *object, Ovl11PaddedVec3 vector);
void ovl_11_func_800D1960(s16 threshold);

s32 Ovl11ApplyStateValueDebit(Ovl11StateValueView *object) {
    s32 difference;
    s32 clamped;

    difference = object->value - (u16)gOvl11StateValueDebit;
    object->value = difference;
    if ((s16)difference < 0) {
        clamped = 0;
    } else {
        clamped = difference;
    }
    object->value = clamped;
    return 0;
}

int Ovl11StateEntryNoop(void) {
    return 0;
}

s32 Ovl11RequestObjectStateMessage(Ovl11D4030Arg *object) {
    s16 selector;
    s16 modeSelector;
    s32 message;
    u16 objectId;

    objectId = object->unk0;
    message = -1;
    switch (objectId) {
    case 0x108:
        selector = object->unk30;
        if (selector == 0x1B) {
            if (GetGameFlag(0x9EU) == 0) {
                SetGameFlag(0x9EU, 1U);
                message = 0x26B;
            } else {
                message = 0x263;
            }
        } else if (selector == 0x1F) {
            if (GetGameFlag(0x9FU) == 0) {
                SetGameFlag(0x9FU, 1U);
                message = 0x26A;
            } else {
                message = 0x264;
            }
        }
        break;
    case 0x162:
        message = 0x261;
        break;
    case 0x165:
        message = 0x262;
        break;
    case 0x17A:
        modeSelector = object->unk30;
        switch (modeSelector) {
        case 0x9:
            message = 0x266;
            if (((struct struct_8006C838_800D4030 *) D_8006C838)->field_99D8 == modeSelector) {
                message = 0x277;
            }
            break;
        case 0x19:
            message = 0x269;
            break;
        case 0x18:
            message = 0x267;
            break;
        case 0x35:
            message = 0x268;
            break;
        }
        break;
    case 0x17B:
        message = 0x265;
        break;
    default:
        message = -1;
        break;
    }
    if (message != -1) {
        RequestMessageWindow(2, message);
    }
    return -1;
}

s32 Ovl11StepState00Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State00Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState01Motion(u16 *record) {
    if (*record != 0x15B) {
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) record,
            Ovl11GetObjectMoveStep(record), 0x8031);
    } else {
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *) record,
            Ovl11GetObjectMoveStep(record), 0x9010);
    }
    if ((*(s32 *) ((u8 *) record + 0x48) == 0) &&
        (*(s32 *) ((u8 *) record + 0x4C) == 0) &&
        (*(s32 *) ((u8 *) record + 0x50) == 0)) {
        *(s16 *) ((u8 *) record + 0x24) = 0;
    } else {
        *(s16 *) ((u8 *) record + 0x24) = 1;
    }
    *(s32 *) ((u8 *) record + 0x34) =
        *(s32 *) ((u8 *) record + 0x34) & ~0x800;
    return 1;
}

s32 Ovl11StepState02CollisionSequence(Ovl11D4314Object *arg0) {
    s16 state;
    s32 result;

    state = arg0->state;
    result = 0;
    switch (state) {
    case 0:
        arg0->state = 1;
        arg0->flags |= 0x800;
        arg0->timer = Rand(60);
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        result = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State02Keyframes, &arg0->state, -1, &arg0->timer)) {
            arg0->state = -1;
        } else {
            switch (arg0->state) {
            case 1:
            case 2:
                if (Ovl11QueryCollisionBudgeted(arg0->position, arg0->collision, 0, 0, 0, 0, 0, 1)) {
                    arg0->state = 4;
                    do {
                        arg0->direction = Rand(4);
                    } while (arg0->direction == 1);
                }
                break;
            case 3:
                if (Rand(100) < 30 && !Ovl11QueryCollisionBudgeted(arg0->position, arg0->collision, 0, 0, 0, 0, 0, 1)) {
                    arg0->state = 1;
                    Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State02Keyframes, &arg0->state, -1, &arg0->timer);
                } else {
                    arg0->state++;
                    do {
                        arg0->direction = Rand(4);
                    } while (arg0->direction == 1);
                }
                break;
            case 4:
                Ovl11SetDirectionVector(arg0->direction, &arg0->velocity, 60);
                arg0->velocity.unk4 -= 16;
                break;
            case 5:
                Ovl11SetDirectionVector(arg0->direction, &arg0->velocity, 60);
                break;
            }
        }
        break;
    }
    return result;
}

s32 Ovl11StepState03DirectedKeyframes(Ovl11DirectedKeyframeObject *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        var_s1 = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State03Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else if (arg0->unk28 == 2) {
            Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0xC);
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState04KeyframesAndClear(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        var_s1 = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State04Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState08Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State08Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState09Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State09Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState11Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State11Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState12Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State12Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState10Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State10Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState13Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State13Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState14DirectedKeyframes(Ovl11DirectedKeyframeObject *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        var_s1 = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State14Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else {
            switch (arg0->unk28) {
            case 2:
                Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0x3C);
                arg0->unk48.unk4 -= 0x10;
                break;
            case 3:
                Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0x3C);
                break;
            }
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState15DirectedKeyframes(Ovl11DirectedKeyframeObject *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        var_s1 = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State15Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else {
            switch (arg0->unk28) {
            case 2:
                Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0x3C);
                arg0->unk48.unk4 -= 0x10;
                break;
            case 3:
                Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0x3C);
                break;
            }
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState16DirectedKeyframes(Ovl11DirectedKeyframeObject *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        Ovl11ClearB0ObjectRecord((s32)arg0);
        var_s1 = 1;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, &gOvl11State16Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        } else if (arg0->unk28 == 2) {
            Ovl11SetDirectionVector(arg0->unk22, &arg0->unk48, 0x3C);
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState17Keyframes(Ovl11D04D4Obj *arg0) {
    s16 temp_v1;
    s32 var_s1;

    temp_v1 = arg0->unk28;
    var_s1 = 0;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        break;
    case -1:
        var_s1 = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, &gOvl11State17Keyframes, &arg0->unk28, -1, (u16 *) &arg0->unk2A) != 0) {
            arg0->unk28 = -1;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11StepState18RecordSelection(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 position;
    s32 result;
    s32 slot;
    s16 state;
    u8 *far_base;

    result = 0;
    state = *(s16 *)((u8 *)arg0 + 0x28);
    switch (state) {
    case 0:
        ((Ovl11ProgressState *)D_8006C838)->flags |= 0x1000000;
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        arg0->field_34 |= 0x800;
        position.field_0 = -0x1324;
        position.field_4 = 0;
        position.field_8 = -0x2198;
        ovl_11_func_800D05D0(arg0, position);
        *(s32 *)((u8 *)arg0 + 0x38) = -0x12C0;
        *(s32 *)((u8 *)arg0 + 0x3C) = -0x12C;
        *(s32 *)((u8 *)arg0 + 0x40) = -0x2EE0;
        arg0->field_30 = 1;
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == 1) {
            PlaySoundEffect(0x1C);
        }
        break;
    case 1:
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        Ovl11StepState01Motion((u16 *)arg0);
        if (!(arg0->field_34 & 0x2000)) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        }
        *(s16 *)((u8 *)arg0 + 0x22) = 2;
        break;
    case 2:
        if (Rand(2) != 0) {
            *(s16 *)((u8 *)arg0 + 0x22) = 2;
        } else {
            *(s16 *)((u8 *)arg0 + 0x22) = 3;
        }
        result = 1;
        *(s16 *)((u8 *)arg0 + 0x16) = 0;
        arg0->field_34 &= ~0x800;
        slot = Ovl11ChooseEligibleTaggedRecord(0);
        if (slot < 0) {
            slot = Ovl11ChooseEligibleTaggedRecord(1);
            *(s16 *)((u8 *)arg0 + 0x16) = 1;
            if (slot < 0) {
                Ovl11EnterObjectState((u16 *)arg0, 0x13);
                break;
            }
        }
        *(u16 *)((u8 *)arg0 + 2) = slot;
        Ovl11EnterObjectState((u16 *)arg0, 0x14);
        break;
    }
    return result;
}

s32 Ovl11StepState19ExitMotion(Ovl11SetFieldsView *arg0) {
    s16 temp_v1;
    s32 var_s2;
    Ovl11PaddedVec3 position;

    var_s2 = 0;
    if ((u16) (*(u16 *) ((u8 *) arg0 + 0xAA)) < 2U) {
        Ovl11StartTimedSpriteEffect((M2C_729a6ccf4038_UnkStruct80107DE0 *) ((u8 *) arg0 + 0xA8), 0x21, 0x5A);
    }
    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x28);
    switch (temp_v1) {
    case 0:
        ((Ovl11ProgressState *) D_8006C838)->flags &= ~0x1000000;
        ((Ovl11ProgressState *) D_8006C838)->flags &= ~0x800000;
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 1;
        arg0->field_34 |= 0x800;
        position.field_0 = -0x1324;
        position.field_4 = 0;
        position.field_8 = -0x22C4;
        ovl_11_func_800D05D0(arg0, position);
        break;
    case 1:
        ovl_11_func_800D1960(0xC8);
        Ovl11StepState01Motion((u16 *) arg0);
        if (!(arg0->field_34 & 0x2000)) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 2;
        }
        arg0->field_34 |= 0x800;
        break;
    case 2:
        (*(s16 *) ((u8 *) arg0 + 0x22)) = 0;
        arg0->field_34 |= 0x200000;
        Ovl11StepState01Motion((u16 *) arg0);
        if (!(arg0->field_34 & 0x200000)) {
            Ovl11SetDirectionVector(0, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0xC);
        }
        if ((*(s32 *) ((u8 *) arg0 + 0x40)) < -0x2EDF) {
            (*(s16 *) ((u8 *) arg0 + 0x28)) = 3;
        }
        arg0->field_34 |= 0x800;
        break;
    case 3:
        arg0->field_34 &= ~0x800;
        Ovl11ClearB0ObjectRecord((s32) arg0);
        var_s2 = 1;
        break;
    }
    return var_s2;
}

s32 Ovl11StepState20RecordFollow(u16 *arg0) {
    s32 kind;
    void *object;
    s32 index;

    if (((struct struct_8006C838_time *)D_8006C838)->field_44C0 < 7) {
        Ovl11EnterObjectState(arg0, 0x13);
    }
    index = Ovl11FindTaggedRecordInBox(*(Ovl11PaddedVec3 *)((u8 *)arg0 + 0x38),
        1, 0x320, &kind, &object);
    if (index != -1) {
        arg0[1] = index;
        if (kind == 2) {
            *(s16 *)((u8 *)arg0 + 0x16) = 1;
        } else {
            *(s16 *)((u8 *)arg0 + 0x16) = 0;
        }
    }
    if (*(s16 *)((u8 *)arg0 + 0x16) == 0) {
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0,
            ((Ovl11D5224LargeView *)((u8 *)D_8006C838 + arg0[1] * 0xB8))->position);
    } else {
        ovl_11_func_800D05D0((Ovl11SetFieldsView *)arg0,
            ((Ovl11D5224SmallView *)((u8 *)D_8006C838 + arg0[1] * 0xB4))->position);
    }
    Ovl11StepState01Motion(arg0);
    if (!(*(s32 *)((u8 *)arg0 + 0x34) & 0x2000)) {
        Ovl11EnterObjectState(arg0, 7);
    }
    return 1;
}

s32 Ovl11StepState07RecordPlacement(Ovl11RecordPlacementStateObject *arg0) {
    s32 sp18[4];
    Ovl11SetFieldsView *var_s0;
    s16 temp_v1;
    s32 (*var_s2)(Ovl11SetFieldsView *, s32);
    s32 var_v0;
    s32 result;
    u8 *far_base;
    s32 x;

    temp_v1 = arg0->state;
    result = 0;
    switch (temp_v1) {
    case 0:
        Ovl11SetDirectionVector(arg0->direction, (Recon800D0408A1View *)&sp18, 0x7D0);
        sp18[0] += arg0->x;
        sp18[1] += arg0->y;
        sp18[2] += arg0->z;
        if ((arg0->pool) == 0) {
            var_s0 = (Ovl11SetFieldsView *) ((u8 *) &gTaggedRecordBase + ((arg0->index) * 0xB8));
            if (var_s0->field_34 & 0x10000) {
                var_s2 = Ovl11DispatchTaggedRecordHandler;
            } else {
                var_s2 = Ovl11DispatchCode160Handler;
            }
        } else {
            var_s2 = 0;
            var_s0 = (Ovl11SetFieldsView *) ((u8 *) &gSmallTaggedRecordBase + ((arg0->index) * 0xB4));
        }
        if (var_s0->field_30 == 1) {
            ovl_11_func_800D05D0(var_s0, *(Ovl11PaddedVec3 *)&sp18);
            (*(u16 *) ((u8 *) var_s0 + 0x22)) = (arg0->direction);
            if (var_s2 != 0) {
                var_s2(var_s0, 3);
            } else {
                Ovl11DispatchSmallRecordHandler((u16 *) var_s0, 3);
            }
        }
        (arg0->state) = 1;
        (arg0->flags) |= 0x800;
        far_base = (u8 *)&D_8007AFF0;
        if ((*(s16 *) (far_base + 0x25476)) == (arg0->map)) {
            PlaySoundEffect(0x1C);
        }
        break;
    case -1:
        result = 1;
        (arg0->timer) = 0x12C;
        (arg0->pool) = 0;
        (arg0->flags) &= ~0x800;
        var_v0 = Ovl11ChooseEligibleTaggedRecord(0);
        if (var_v0 < 0) {
            var_v0 = Ovl11ChooseEligibleTaggedRecord(1);
            (arg0->pool) = 1;
            if (var_v0 < 0) {
                Ovl11EnterObjectState((u16 *)arg0, 0x13);
                break;
            }
        }
        (arg0->index) = var_v0;
        Ovl11EnterObjectState((u16 *)arg0, 0x14);
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *) arg0, &gOvl11State07Keyframes, (s16 *) ((u8 *)arg0 + 0x28), -1, (u16 *)((u8 *)arg0 + 0x2A)) != 0) {
            (arg0->state) = -1;
        }
        break;
    }
    return result;
}

s32 Ovl11StepState06TimedSprite(Ovl11D55F8Object *arg0) {
    s32 result;
    u8 *far_base;

    result = 0;
    switch (arg0->unk28) {
    case 0:
        Ovl11StartTimedSpriteEffect((Ovl11C35ECStatePair *)((u8 *)arg0 + 0xA8), 0x24, 0x2D);
        arg0->unk28 = 1;
        arg0->unk2C = 0;
        arg0->unk2A = 0;
        arg0->unk22 = (u16)gOvl11StateEntryDirection;
        arg0->unk34 = (arg0->unk34 & ~0x2000) | 0x800;
        ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x18, 0x9010);
        far_base = (u8 *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
            PlaySoundEffect(0x1A);
        }
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (arg0->unk2C != 0) {
            arg0->unk2C--;
        }
        if (Ovl11StepPackedKeyframes((Ovl11D04D4Obj *)arg0, gOvl11State06Keyframes, &arg0->unk28, -1, (u16 *)&arg0->unk2C) != 0) {
            arg0->unk28 = -1;
        } else {
            ovl_11_func_800D12B8((Recon_ovl_11_func_800D2594_A0View *)arg0, 0x18, 0x9010);
        }
        break;
    }
    return result;
}
