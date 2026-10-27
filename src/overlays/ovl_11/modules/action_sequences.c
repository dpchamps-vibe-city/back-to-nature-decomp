/*
 * action_sequences — reconstructed translation unit.
 *
 * Object action sequences sharing grid-target acquisition, record selection,
 * keyed placement, state-counter probes and completion.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_action_sequences.h"
#include "ovl11_area_slot_flags.h"
#include "ovl11_save_counters.h"
#include "ovl11_record_position_tables.h"
#include "psyq/stddef.h"
#include "ovl11_tagged_record_handlers.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_seasonal_record_ids.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_eligible_record_selection.h"
#include "sound_commands.h"
#include "game_flags.h"
#include "ovl11_collision_query_budget.h"
#include "ovl11_cell_actions.h"
#include "ovl11_grid_position.h"
#include "ovl11_direction_vector.h"
#include "ovl11_cell_geometry.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_record_movement.h"
#include "ovl11_object_mode_handlers.h"
#include "ovl11_record_position_set.h"

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

s32 ovl_11_func_800F3BCC(u16 arg0);

u16 Ovl11CompleteActionSequence(Ovl11ActionCompletionView *object) {
    char *farBase;

    ovl_11_func_800F3BCC(object->actionId);
    farBase = (char *)&D_8007AFF0;
    if (*(s16 *)(farBase + 0x25476) == object->area) {
        PlaySoundEffect(0x13);
        SetGameFlag(7, 1);
    }
    object->actionId = 0;
    object->actionState = 0;
    return 1;
}

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

s32 Ovl11AdvanceActionMovement(Ovl11E644Object *arg0, s16 arg1) {
    s32 temp_s0;

    temp_s0 = arg0->unk34;
    Ovl11SetMovementRankThreshold(arg1);
    Ovl11StepObjectMode7Movement(arg0);
    if (temp_s0 & 0x800) {
        arg0->unk34 |= 0x800;
    }
    if (!(arg0->unk34 & 0x2000)) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *) &arg0->unk48, 0);
        return 1;
    }
    return 0;
}

s32 Ovl11AcquireActionGridTarget(Ovl11SetFieldsView *arg0, s32 arg1, s32 arg2) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 sp2A;
    s32 temp;

    temp = arg1 != 1;
    if (Ovl11FindCandidateGridCell(&sp28, &sp2A, arg1, arg2, 1) != 0) {
        return 1;
    }
    Ovl11GetCellCornerPosition(sp28, sp2A, (Vec3 *) &sp18, temp);
    Ovl11SetRecordPosition(arg0, sp18);
    return 0;
}
