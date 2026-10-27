/* Action-entry scripts and record handoffs, ovl_11:80109EC4–8010B05C.
 * The complete 19-slot bank points into this run; compound handlers share
 * motion refresh and the final spawn handoff. Update-phase handlers belong
 * to the other bank. Membership/boundaries: notes/file-groupings.md.
 *
 * Keep the separate callback bodies and original order: slots can share an
 * implementation (8/9, 4/17), but distinct scripts remain distinct callbacks.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_action_state_handlers.h"
#include "sound_commands.h"
#include "ovl11_record_pair_order.h"
#include "ovl11_object_state_dispatch.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_keyframe_step.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_direction_vector.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_object_state_callbacks.h"
#include "ovl11_record_movement.h"
#include "psyq/stddef.h"
#include "ovl11_fixed_spawn_record_handoff.h"
#include "ovl11_record_position_set.h"

s32 ovl_11_func_80109188(Ovl11D04D4Obj *object, s32 mode);
s32 Ovl11TryActionSpawnHandoff(Ovl11SetFieldsView *object);

s32 Ovl11StepActionKeyframes0(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry0Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes2(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry2Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes1(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry1Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes8(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry8Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes10(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry10Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes11(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry11Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes12(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry12Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes13(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry13Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepFoundRecordAction(Ovl11A47CObject *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11FoundRecordActionTimer = 0;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, gOvl11FoundRecordActionKeyframes, &object->unk28, -1, &gOvl11FoundRecordActionTimer) != 0) {
            object->unk28 = -1;
        }
        if (object->unk34 & 0x2000) {
            object->unk58 = gOvl11ActionFoundRecord[0x38 / 4];
            object->unk5C = gOvl11ActionFoundRecord[0x3C / 4];
            object->unk60 = gOvl11ActionFoundRecord[0x40 / 4];
            object->unk64 = gOvl11ActionFoundRecord[0x44 / 4];
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            if (object->unk28 != 1) {
                object->unk28 = -1;
            }
        } else {
            if (gOvl11ActionFoundRecordCategory != 0) {
                Ovl11DispatchCode160Handler((Ovl11ActionDispatchView *)gOvl11ActionFoundRecord, 0xC);
            } else {
                Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)gOvl11ActionFoundRecord, 0xC);
            }
            if (object->unk28 == 1) {
                gOvl11FoundRecordActionTimer = 0;
                object->unk28++;
                if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                    PlaySoundEffect(0x19);
                }
            }
            Ovl11SelectRecordPairCode((Recon_ovl_11_func_800D2594_A0View *)object);
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepProbedRecordAction(Ovl11A654Object *object) {
    s16 phase;
    s32 completed;

    completed = 0;
    phase = object->unk28;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11ProbedRecordActionTimer = 0;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        ovl_11_func_80109188((Ovl11D04D4Obj *)object, 0);
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes((Ovl11A47CObject *)object, gOvl11ProbedRecordActionKeyframes, &object->unk28, -1, &gOvl11ProbedRecordActionTimer) != 0) {
            object->unk28 = -1;
        }
        if (object->unk34 & 0x2000) {
            object->unk58 = gOvl11ActionProbedRecord[0x38 / 4];
            object->unk5C = gOvl11ActionProbedRecord[0x3C / 4];
            object->unk60 = gOvl11ActionProbedRecord[0x40 / 4];
            object->unk64 = gOvl11ActionProbedRecord[0x44 / 4];
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            if (object->unk28 != 1) {
                object->unk28 = -1;
            }
        } else {
            if (object->unk28 == 1) {
                if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                    PlaySoundEffect(0x19);
                }
                Ovl11SetStateDebitAndDirection(((s16)object->unkAC / 10) & 0xFFFF, object->unk22);
                Ovl11EnterObjectState((u16 *)gOvl11ActionProbedRecord, 6);
                gOvl11ProbedRecordActionTimer = 0;
                object->unk28 = (u16)object->unk28 + 1;
            }
            Ovl11SelectRecordPairCode((Recon_ovl_11_func_800D2594_A0View *)object);
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepActionRecordHandoff(Ovl11A47CObject *object) {
    s32 completed;
    s32 motionFlag;
    u32 handoffStatus;

    motionFlag = object->unk34 & 0x01000000;
    completed = 0;
    switch (object->unk28) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        gOvl11RecordHandoffTimer = 0;
        gOvl11RecordHandoffPhase = 0;
        break;
    case -3:
        if (object->unk34 & 0x01000000) {
            gOvl11ActionMotionVariant = 1;
            Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
            break;
        }
        /* fallthrough */
    case -2:
        Ovl11PublishObjectToFixedSpawnRecord((Ovl11SpawnHandoffObject *)object);
        /* fallthrough */
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        object->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, gOvl11RecordHandoffKeyframes, &object->unk28, -1, &gOvl11RecordHandoffTimer)) {
            object->unk28 = -2;
        }
        switch (object->unk28) {
        case 1:
            handoffStatus = Ovl11TryActionSpawnHandoff((Ovl11SetFieldsView *)object);
            switch (handoffStatus) {
            case 1:
                object->unk28++;
                gOvl11RecordHandoffPhase = 1;
                break;
            case 0:
                break;
            case 2:
                object->unk28 = -1;
                break;
            }
            break;
        case 2:
            if ((u32)((u16)gOvl11RecordHandoffPhase - 1) >= 2U) {
                object->unk28 = -1;
                break;
            }
            gOvl11ActionMotionVariant = 1;
            Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *)(((struct Ovl11A850State *)D_8006C838)->record + 0x18), NULL);
            Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, ((struct Ovl11A850State *)D_8006C838)->position);
            if (object->unk7A & 0x300) {
                if (gOvl11RecordHandoffPhase == 2) {
                    object->unk28++;
                }
                gOvl11RecordHandoffPhase = 2;
            }
            break;
        case 3:
            if (gOvl11RecordHandoffPhase == 2) {
                if (object->unk34 & 0x2000) {
                    Ovl11SetRecordPosition((Ovl11SetFieldsView *)object, ((struct Ovl11A850State *)D_8006C838)->position);
                    gOvl11ActionMotionVariant = 1;
                    Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
                } else {
                    if (!motionFlag) {
                        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
                    }
                    object->unk28++;
                    gOvl11RecordHandoffTimer = 0;
                    gOvl11RecordHandoffPhase = 3;
                    if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == object->unk30) {
                        PlaySoundEffect(0x19);
                    }
                }
            } else {
                object->unk28 = -1;
            }
            break;
        case 4:
            if ((gOvl11RecordHandoffPhase != 3) || motionFlag) {
                object->unk28 = -3;
            }
            break;
        }
        object->unk34 |= 0x800;
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes14(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry14Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes15(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry15Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionKeyframes16(Ovl11D04D4Obj *object) {
    s16 phase;
    s32 completed;

    phase = object->unk28;
    completed = 0;
    switch (phase) {
    case 0:
        object->unk28 = 1;
        object->unk34 |= 0x800;
        break;
    case -1:
        completed = 1;
        object->unk2C = 0x12C;
        object->unk34 &= ~0x800;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(object, &gOvl11ActionEntry16Keyframes, &object->unk28, -1, (u16 *)&object->unk2A) != 0) {
            object->unk28 = -1;
        }
        break;
    }
    return completed;
}

s32 Ovl11StepActionMovement(Recon_ovl_11_func_800D2594_A0View *object) {
    object->unk34 &= 0xFF7FFFFF;
    Ovl11StepRecordMovement(object, 0xC, 0x31);
    if ((*(s32 *)((u8 *)object + 0x48) == 0) && (object->unk4C == 0) && (*(s32 *)((u8 *)object + 0x50) == 0)) {
        *(s16 *)((u8 *)object + 0x24) = 0;
    } else {
        *(s16 *)((u8 *)object + 0x24) = 1;
    }
    object->unk34 &= ~0x800;
    return 1;
}

s32 Ovl11RefreshActionMotion(Recon_ovl_11_func_800D2594_A0View *object) {
    u16 *animations;
    s32 stepSize = 0x18;

    animations = gOvl11ActionMotionAnimations;
    if (gOvl11ActionMotionVariant != 0) {
        animations += 3;
    }
    if (object->unk34 & 0x8000) {
        object->unk34 |= 0x800000;
        stepSize = 0x24;
    }
    Ovl11StepRecordMovement(object, stepSize, 0x31);
    object->unk34 |= 0x800;
    if ((*(s32 *)((u8 *)object + 0x48) == 0) && (object->unk4C == 0) && (*(s32 *)((u8 *)object + 0x50) == 0)) {
        if (*(u16 *)((u8 *)object + 0x7A) & 0x300) {
            *(u16 *)((u8 *)object + 0x24) = animations[0];
        } else {
            *(u16 *)((u8 *)object + 0x24) = animations[1];
        }
    } else if (object->unk34 & 0x01000000) {
        *(u16 *)((u8 *)object + 0x24) = animations[2];
    } else {
        *(u16 *)((u8 *)object + 0x24) = animations[1];
    }
    if (*(u16 *)((u8 *)object + 0x7A) & 0x300) {
        if (!(object->unk34 & 0x01000000)) {
            object->unk34 &= ~0x800;
        }
    }
    if (!(object->unk34 & 0x2000)) {
        *(s16 *)((u8 *)object + 0x2C) = 0x12C;
        if ((*(u16 *)((u8 *)object + 0x7A) & 0x300) && !(object->unk34 & 0x01000000)) {
            Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
        }
    }
    return 1;
}

/* Only the record-handoff callback calls this helper. Flag 2000 keeps the
 * target position synchronized to spawn 65; absence means clear direction.
 * Results are the caller's protocol: 0 = tracking, 1 = released, 2 = no spawn.
 */
s32 Ovl11TryActionSpawnHandoff(Ovl11SetFieldsView *object) {
    Ovl11PaddedVec3 position;
    u16 spawnKind;

    if (object->field_34 & 0x2000) {
        if (Ovl11ReadSpawnRecord(0x65, &position.vx, &spawnKind, 0) == 0) {
            return 2;
        }
        Ovl11SetRecordPosition(object, position);
        Ovl11RefreshActionMotion((Recon_ovl_11_func_800D2594_A0View *)object);
        return 0;
    }
    Ovl11SetDirectionVector(4, (Recon800D0408A1View *)((u8 *)object + 0x48), 0);
    return 1;
}
