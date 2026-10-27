/*
 * Installed condition-record evaluation and result queueing. The driver
 * selects the pending state, then short-circuits calendar, range, item and
 * flag predicates in record order. Tests may consume pending work state;
 * queueing failures do not stop the record walk.
 *
 * The mask/field decoders belong to this evaluator's helper closure, not
 * separate files. Their existing headers retain the packed-field contracts.
 * The installer at 800F0E00 belongs to the same data protocol but remains
 * outside only because its strength-reduction setting is unresolved.
 * Public lifecycle/side effects: ovl11_condition_evaluation.h.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_condition_masks.h"
#include "ovl11_condition_record_fields.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_object_events.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_pending_item_command.h"
#include "ovl11_condition_field_read.h"

s32 Ovl11EvaluateConditionRecords(Ovl11ConditionRecord *arg0, s32 arg1);

s32 Ovl11EvaluatePendingConditions(void) {
    if (((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x800) {
        return 1;
    }
    if (gOvl11ConditionEvaluationState != 0) {
        Ovl11EvaluateConditionRecords((Ovl11ConditionRecord *) ((u8 *) D_8006C838 + 0x2DB8), gOvl11ConditionRecordCount);
    }
    gOvl11ConditionEvaluationState = 0;
    return 1;
}

void Ovl11SetConditionState(s32 state) {
    if ((gOvl11ConditionEvaluationState == 1) || (GetGameFlag(2U) == 0)) {
        gOvl11ConditionEvaluationState = state;
    }
    if ((D_80070CF8_word == 6) && (Ovl11HasQueuedClockEvent() != 0) && (gOvl11ConditionEvaluationState != 8)) {
        gOvl11ConditionEvaluationState = 0;
    }
}

s32 Ovl11QueueConditionResultAnySlot(s32 arg0, s32 arg1);

void Ovl11QueueConditionCodeAnySlot(u16 code, s32 payload) {
    Ovl11QueueConditionResultAnySlot(code, payload);
}

s32 Ovl11QueueConditionResultFirstSlot(s32 arg0, s32 arg1);

void Ovl11QueueConditionCodeFirstSlot(u16 code, s32 payload) {
    Ovl11QueueConditionResultFirstSlot(code, payload);
}

s32 Ovl11ConditionStateMatches(u8 arg0, u8 arg1);
s32 Ovl11ConditionDayWindowMatches(s32 arg0, s32 arg1, s32 arg2);
s32 Ovl11ConditionHourWindowMatches(u16 arg0, u16 arg1, s16 *arg2);
s32 Ovl11ConditionHeaderKindMatches(u16 arg0, s32 arg1);
s32 Ovl11ConditionValueRangeMatches(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Ovl11ConditionSelectionFlagsMatch(u8 arg0);
s32 Ovl11ConditionCounterMatches(u8 arg0, u16 arg1, s32 arg2);
s32 Ovl11ConditionFlagIdsMatch(u32 arg0, s32 arg1);
s32 Ovl11ConditionFlagPairMatches(s32 arg0);
s32 Ovl11ConditionCodeIsAvailable(s32 arg0);
s32 Ovl11ConditionItemListMatches(u8 arg0);

s32 Ovl11EvaluateConditionRecords(Ovl11ConditionRecord *record, s32 count) {
    u16 bounds[6];
    s32 index;

    for (index = 0; index < count; index++, record++) {
        ((struct Ovl11ConditionEvaluationWork *)D_8006C838)->current = record;
        if (((record->stateCode != 2) ||
             !(((struct Ovl11ConditionObjects *)&D_8007AFF0)->second->flags & 0x1000) ||
             !(((struct Ovl11ConditionObjects *)&D_8007AFF0)->first->flags & 0x1000) ||
             ((u32)((record->codeAndKind & 0x3FF) - 0xF0) < 10U)) &&
            (Ovl11ConditionStateMatches(record->stateCode, record->stateArgument) != 0)) {
            Ovl11DecodeConditionBounds(record->packedBounds, bounds);
            if ((((struct Ovl11ConditionEvaluationWork *)D_8006C838)->date[0] >= bounds[0]) &&
                (bounds[1] >= ((struct Ovl11ConditionEvaluationWork *)D_8006C838)->date[0]) &&
                (Ovl11ConditionDayWindowMatches(bounds[2], bounds[3], (s32)((struct Ovl11ConditionEvaluationWork *)D_8006C838)->date) != 0) &&
                (Ovl11ConditionHourWindowMatches(bounds[4], bounds[5], ((struct Ovl11ConditionEvaluationWork *)D_8006C838)->time) != 0) &&
                (Ovl11ConditionHeaderKindMatches(record->codeAndKind, record->conditions) != 0) &&
                (Ovl11ConditionValueRangeMatches(record->valueField, record->valueGuard, record->lowerBound, record->upperBound, record->conditions) != 0) &&
                (Ovl11ConditionSelectionFlagsMatch(record->selectionFlags) != 0) &&
                (Ovl11ConditionCounterMatches(record->counterSelector, record->counterThreshold, record->conditions) != 0) &&
                (Ovl11ConditionFlagIdsMatch(record->requiredFlags, 1) != 0) &&
                (Ovl11ConditionFlagIdsMatch(record->clearFlags, 0) != 0) &&
                (Ovl11ConditionFlagPairMatches(record->flagPair) != 0) &&
                (Ovl11TestConditionMask(record->conditions) != 0) &&
                (Ovl11ConditionCodeIsAvailable(record->codeAndKind) != 0) &&
                (Ovl11ConditionItemListMatches(record->itemListSelector) != 0)) {
                Ovl11QueueConditionResultFirstSlot(record->codeAndKind & 0x3FF, (u8)(record->conditions >> 8) >> 7);
            }
        }
    }
    return 1;
}

/* Both reservation helpers return 0 for a queued result, 1 for suppression
 * or a full slot set. The evaluator intentionally ignores that result. */
s32 Ovl11QueueConditionResultFirstSlot(s32 arg0, s32 arg1) {
    u16 temp_s0;
    char *temp_a1;
    char *far_base;

    temp_s0 = arg0 & 0xFFFF;
    if ((D_80070C88 & 1) && (temp_s0 < 0x12C)) {
        return 1;
    }
    temp_a1 = Ovl11ReserveFirstRecordSlotIfAbsent(0);
    if (temp_a1 != 0) {
        *(u16 *)(temp_a1 + 2) = temp_s0;
        *(u16 *)(temp_a1 + 6) = arg1;
        far_base = (char *)&D_8007AFF0;
        *(u16 *)(temp_a1 + 4) = *(u16 *)(far_base + 0x25476);
        return 0;
    }
    return 1;
}

s32 Ovl11QueueConditionResultAnySlot(s32 arg0, s32 arg1) {
    u16 temp_s0;
    char *temp_a1;
    char *far_base;

    temp_s0 = arg0 & 0xFFFF;
    if ((D_80070C88 & 1) && (temp_s0 < 0x12C)) {
        return 1;
    }
    temp_a1 = Ovl11ReserveAnyRecordSlot(0);
    if (temp_a1 != 0) {
        *(u16 *)(temp_a1 + 2) = temp_s0;
        *(u16 *)(temp_a1 + 6) = arg1;
        far_base = (char *)&D_8007AFF0;
        *(u16 *)(temp_a1 + 4) = *(u16 *)(far_base + 0x25476);
        return 0;
    }
    return 1;
}

s32 Ovl11HasInactiveConditionRowCode(u16 arg0) {
    s32 found;
    s32 i;

    found = 0;
    for (i = 0; i < 5; i++) {
        if (((D8006C838RecordTableView *)D_8006C838)->records[i][0] == 0) {
            if (((D8006C838RecordTableView *)D_8006C838)->records[i][1] == arg0) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

/* Zero length requires the exact hour AND minute zero; 31 bypasses. */
s32 Ovl11ConditionHourWindowMatches(u16 arg0, u16 arg1, s16 *arg2) {
    s32 temp_a0;
    s32 var_a2;
    s32 var_t0;
    s32 var_v1;
    u32 var_a1;
    s32 temp_a3;

    temp_a0 = arg0 & 0xFFFF;
    var_a1 = arg1 & 0xFFFF;
    if ((temp_a0 == 0x1F) || (var_a1 == 0x1F)) {
        return 1;
    }
    temp_a3 = arg2[0];
    if (var_a1 >= 0x19U) {
        var_a1 = 0x18;
    }
    var_t0 = 0;
    if (var_a1 != 0) {
        for (var_a2 = 0; var_a2 < (s32) var_a1; var_a2++) {
            var_v1 = temp_a0 + var_a2;
            if (var_v1 >= 0x18) {
                var_v1 -= 0x18;
            }
            if (var_v1 == temp_a3) {
                var_t0 = 1;
                break;
            }
        }
    } else if ((temp_a0 == temp_a3) && (arg2[1] == 0)) {
        var_t0 = 1;
    }
    return var_t0;
}

/* The observed wrap subtracts 29 once at >=31. This is deliberately not
 * replaced with a conventional modulo-30/31 calendar calculation. */
s32 Ovl11ConditionDayWindowMatches(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 x;
    s32 target;
    s32 found;

    arg0 &= 0xFFFF;
    arg1 &= 0xFFFF;
    if ((arg0 == 31) || (arg1 == 31)) {
        return 1;
    }
    found = 0;
    target = *(s16 *)(arg2 + 4);
    if ((u32)arg1 >= 30U) {
        arg1 = 29;
    }
    i = 0;
    arg1++;
    for (; i < arg1; i++) {
        x = arg0 + i;
        if (x >= 31) {
            x -= 29;
        }
        if (x == target) {
            found = 1;
            break;
        }
    }
    return found;
}

s32 Ovl11ConditionHeaderKindMatches(u16 arg0, s32 arg1) {
    s32 kind;

    kind = arg0 >> 10;
    if (kind == 0x3F) {
        return 1;
    }
    return (arg1 < 0) ? (kind != ((Ovl11WorkHeaderView *)&D_8007AFF0)->header->unk2)
                      : (kind == ((Ovl11WorkHeaderView *)&D_8007AFF0)->header->unk2);
}

/* Encoded inputs use byte-sized field/index IDs and 0xFF open bounds.
 * Only the read result's low byte participates; bit 0x400 negates the test. */
s32 Ovl11ConditionValueRangeMatches(s32 fieldId, s32 elementIndex, s32 minimum, s32 maximum, s32 flags) {
    s32 matches;
    s32 valueForComparison;
    u32 field;
    u32 element;
    u32 lower;
    u32 upper;
    u32 value;

    field = fieldId & 0xFF;
    element = elementIndex & 0xFF;
    lower = minimum & 0xFFFF;
    upper = maximum & 0xFFFF;
    if ((field == 0xFF) || (element == 0xFF)) {
        value = flags & 0x400;
        return value == 0;
    }
    value = Ovl11ReadConditionField(field, element);
    matches = 0;
    value = value & 0xFF;
    if (lower == 0xFF) {
        if (upper == lower) {
            matches = 1;
        } else if (upper >= value) {
            matches = 1;
        }
    } else if (upper == 0xFF) {
        if (value >= lower) {
            matches = 1;
        }
    } else {
        valueForComparison = (u16)value;
        if (valueForComparison >= lower) {
            if (upper >= valueForComparison) {
                matches = 1;
            }
        }
    }
    if (flags & 0x400) {
        return matches ^ 1;
    }
    return matches;
}

s32 Ovl11ConditionSelectionFlagsMatch(u8 arg0) {
    s32 id;
    s32 flags;
    s16 index;
    char *record;
    char *base;

    flags = arg0;
    if (((Ovl11EF488SelectionView *)&D_8006C838)->selected == -1) {
        if (!(flags & 0x40)) {
            goto zero;
        }
    } else if (!(flags & 0x80)) {
        goto zero;
    }
    id = flags & 7;
    if (id == 7) {
        return 1;
    }
    index = Ovl11GetConditionRecordSlot(id);
    base = (char *)&D_8006C838;
    record = base + index * 0x1D4;
    if (!(*(u16 *)(record + 0x9A06) & 8)) {
        goto other;
    }
    if (flags & 0x20) {
        goto one;
    }
zero:
    return 0;
other:
    if (!(flags & 0x10)) {
        return 0;
    }
one:
    return 1;
}

s32 Ovl11ConditionCounterMatches(u8 arg0, u16 arg1, s32 arg2) {
    s32 temp_s0;
    s32 var_a0;
    u16 var_v1;
    u32 temp_s1;

    temp_s0 = arg0 & 0xFF;
    temp_s1 = arg1 & 0xFFFF;
    if (temp_s0 == 0xFF) {
        return 1;
    }
    if (temp_s0 == 0x32) {
        char *base = (char *) &D_8006C838;
        var_v1 = *(u16 *) (base + 0x8000 + 0x12B2);
    } else if (temp_s0 == 0x33) {
        char *base = (char *) &D_8006C838;
        var_v1 = *(u16 *) (base + 0x8000 + 0x11C2);
    } else {
        if (Ovl11IsSpecialCounterSlot(temp_s0) == 0) {
            char *base = (char *) &D_8006C838;
            char *q = base + temp_s0 * 0x1D4;
            var_v1 = *(u16 *) (q + 0x8000 + 0x19EA);
        } else {
            char *base = (char *) &D_8006C838;
            char *q = base + temp_s0 * 0x1D4;
            var_v1 = *(u16 *) (q + 0x8000 + 0x19EC);
        }
    }
    var_a0 = 0;
    if (arg2 & 0x4000) {
        var_a0 = temp_s1 >= var_v1;
    } else if (var_v1 >= temp_s1) {
        var_a0 = 1;
    }
    return var_a0;
}

s32 Ovl11ConditionFlagIdsMatch(u32 arg0, s32 arg1) {
    s16 flagIds[3];
    s16 *var_s0;
    s32 var_s1;
    s32 var_s2;
    u16 temp_a0;
    u16 mask;
    s32 one;

    Ovl11DecodeConditionFlagIds(arg0, flagIds, arg1);
    var_s1 = 1;
    var_s2 = 0;
    mask = 0x3FF;
    var_s0 = flagIds;
    for (;;) {
        one = 1;
        if (arg1 == one) {
            temp_a0 = (u16) *var_s0;
            if ((temp_a0 != mask) && (GetGameFlag((temp_a0 + 0xFB) & 0xFFFF) != arg1)) {
                var_s1 = 0;
            }
        } else {
            temp_a0 = (u16) *var_s0;
            if ((temp_a0 != mask) && (GetGameFlag((temp_a0 + 0xFB) & 0xFFFF) != 0)) {
                var_s1 = 0;
            }
        }
        var_s2 += 1;
        if (var_s1 == 0) {
            break;
        }
        if (var_s2 < 3) {
            var_s0 += 1;
            continue;
        }
        break;
    }
    return var_s1;
}

s32 Ovl11ConditionFlagPairMatches(s32 arg0) {
    s32 v;
    s32 one = 1;
    s32 f;

    f = arg0 & 0x3FF;
    if (f != 0x3FF) {
        v = GetGameFlag(f + 0xC8);
        if (v != one) {
            return 0;
        }
    }
    f = ((u32) arg0 >> 10) & 0x3FF;
    if (f != 0x3FF) {
        v = GetGameFlag(f + 0xC8);
        if (v != 0) {
            return 0;
        }
    }
    return 1;
}

s32 Ovl11ConditionObjectStateMatches(s32 arg0) {
    return D_80071A22 == (arg0 & 0xFF);
}

s32 Ovl11ConditionObjectStateMatches(s32 arg0);

s32 Ovl11ConditionStateMatches(u8 arg0, u8 arg1) {
    s32 result;

    if (arg0 == 1) {
        return 1;
    }
    result = 0;
    if (arg0 == gOvl11ConditionEvaluationState) {
        result = 1;
        switch (arg0) {
        case 2:
            break;
        case 3:
            if (Ovl11ConditionObjectStateMatches(arg1) == 0) {
                result = 0;
            }
            break;
        case 4:
            if (((struct Ovl11E9914State *)D_8006C838)->pending != 0x186) {
                result = Ovl11ConsumePendingItemCommand(0, arg1, -1, 0);
                ((struct Ovl11E9914State *)D_8006C838)->pending = 0;
            } else {
                result = 0;
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            break;
        }
    }
    return result;
}

s32 ovl_11_func_80104EC8(s16 arg0);

s32 Ovl11ConditionItemListMatches(u8 arg0) {
    s32 result;

    if (arg0 != 0xFF) {
        result = ovl_11_func_80104EC8(arg0);
    } else {
        result = 1;
    }
    return result;
}

s32 Ovl11HasInactiveConditionRowCode(u16 arg0);

s32 Ovl11ConditionCodeIsAvailable(s32 arg0) {
    s32 var_s0;
    s32 callRet2;

    var_s0 = arg0 & 0x3FF;
    callRet2 = GetGameFlag(var_s0 + 0xFB);
    if (callRet2 == 1) {
        return 0;
    }
    return Ovl11HasInactiveConditionRowCode(var_s0) == 0;
}

s32 Ovl11TestConditionMask(s32 conditions) {
    if (Ovl11MaskAllowsSeason(conditions) == 0) {
        return 0;
    }
    if (Ovl11MaskAllowsWeekday(conditions) == 0) {
        return 0;
    }
    if (Ovl11MaskAllowsDayCondition(conditions) == 0) {
        return 0;
    }
    if (conditions & 0x20000) {
        if (Ovl11MatchItemSlotsCommand(0, 0, 1, 0) == 0) {
            return 0;
        }
    }
    if (conditions & 0x10000) {
        if (Ovl11MatchItemSlotsCommand(0, 1, 0, 0) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 Ovl11MaskAllowsSeason(s32 conditions) {
    s32 selectedBit = 0;

    switch (gCalendarSeason) {
    case 0:
        selectedBit = 0x40000000;
        break;
    case 1:
        selectedBit = 0x20000000;
        break;
    case 2:
        selectedBit = 0x10000000;
        break;
    case 3:
        selectedBit = 0x08000000;
        break;
    }
    return (selectedBit & conditions) != 0;
}

s32 Ovl11MaskAllowsWeekday(s32 conditions) {
    s32 selectedBit = 0;

    switch (gCalendarWeekday) {
    case 0:
        selectedBit = 0x4000000;
        break;
    case 1:
        selectedBit = 0x2000000;
        break;
    case 2:
        selectedBit = 0x1000000;
        break;
    case 3:
        selectedBit = 0x800000;
        break;
    case 4:
        selectedBit = 0x400000;
        break;
    case 5:
        selectedBit = 0x200000;
        break;
    case 6:
        selectedBit = 0x100000;
        break;
    }
    return (selectedBit & conditions) != 0;
}

s32 Ovl11MaskAllowsDayCondition(s32 conditions) {
    char *work;
    s32 selectedBit;

    /* An unrestricted field must also accept unknown current codes. */
    selectedBit = conditions & 0xC3800;
    if (selectedBit == 0xC3800) {
        return 1;
    }

    /* Keep the shared-work origin explicit rather than introducing a second
     * global identity for this interior halfword. */
    work = (char *)D_8006C838;
    selectedBit = 0;
    switch (*(s16 *)(work + 0xE4C8)) {
    case 0:
        selectedBit = 0x80000;
        break;
    case 2:
        selectedBit = 0x40000;
        break;
    case 1:
        selectedBit = 0x2000;
        break;
    case 3:
        selectedBit = 0x1000;
        break;
    case 4:
        selectedBit = 0x800;
        break;
    }

    return (selectedBit & conditions) != 0;
}

/*
 * Pure packed-field decoders for the condition-record evaluator.
 * The bounds and flag-ID words use different bit orders and sentinel rules;
 * keep those formats distinct. The switches retain the original pooled-table
 * order; the flag-ID selector's two identical branches are intentional.
 * Public contracts: ovl11_condition_record_fields.h.
 */
void Ovl11DecodeConditionBounds(u32 packed, u16 *bounds) {
    s32 fieldIndex;
    u16 value;
    u16 limit;

    for (fieldIndex = 0; fieldIndex < 6; fieldIndex++) {
        switch (fieldIndex) {
        case 0:
            value = (packed >> 25) & 0x1F;
            limit = 1;
            break;
        case 1:
            value = (packed >> 20) & 0x1F;
            limit = 0x7FFF;
            break;
        case 2:
            value = (packed >> 15) & 0x1F;
            limit = 0x1F;
            break;
        case 3:
            value = (packed >> 10) & 0x1F;
            limit = 0x1F;
            break;
        case 4:
            value = (packed >> 5) & 0x1F;
            limit = 0x1F;
            break;
        case 5:
            value = packed & 0x1F;
            limit = 0x1F;
            break;
        default:
            value = 0;
            limit = 0;
            break;
        }
        if (value == 0x1F) {
            value = limit;
        }
        switch (fieldIndex) {
        case 0:
            bounds[fieldIndex] = value - 1;
            break;
        case 1:
            bounds[fieldIndex] = value - 1;
            break;
        case 2:
            bounds[fieldIndex] = value - 1;
            break;
        case 3:
            bounds[fieldIndex] = value;
            break;
        case 4:
            bounds[fieldIndex] = value;
            break;
        case 5:
            bounds[fieldIndex] = value;
            break;
        default:
            bounds[fieldIndex] = value;
            break;
        }
    }
}

void Ovl11DecodeConditionFlagIds(u32 packed, s16 *ids, s32 selector) {
    s32 fieldIndex;
    s32 value;

    for (fieldIndex = 0; fieldIndex < 3; fieldIndex++) {
        if (selector == 1) {
            switch (fieldIndex) {
            case 0:
                value = (u16)(packed & 0x3FF);
                break;
            case 1:
                value = (u16)((packed >> 10) & 0x3FF);
                break;
            case 2:
                value = (u16)((packed >> 20) & 0x3FF);
                break;
            default:
                value = 0;
                break;
            }
        } else {
            switch (fieldIndex) {
            case 0:
                value = (u16)(packed & 0x3FF);
                break;
            case 1:
                value = (u16)((packed >> 10) & 0x3FF);
                break;
            case 2:
                value = (u16)((packed >> 20) & 0x3FF);
                break;
            default:
                value = 0;
                break;
            }
        }
        *ids++ = value;
    }
}

s32 Ovl11GetConditionRecordSlot(s32 selector) {
    u32 selector16;

    selector16 = selector & 0xFFFF;
    switch (selector16) {
    case 0:
        return 3;
    case 1:
        return 7;
    case 2:
        return 9;
    case 3:
        return 0xE;
    case 4:
        return 0x14;
    default:
        return 3;
    }
}
