/*
 * ID-keyed action guards and area-specific exceptions.
 *
 * Records pair typed predicates with a result code. The private selector
 * couples successive records; these calls are not pure Boolean expressions.
 * Public lookup contract: ovl11_action_guards.h. The raw list-header view keeps
 * the witnessed signed halfword ID/count and pointer at +4. Unknown field and
 * selector meanings are left neutral. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_action_guards.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"
#include "psyq/abs.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_events.h"

s32 Ovl11IsActionGuardAreaBlocked(s32 id);
s32 Ovl11EvaluateActionGuardList(s16 *header);
s32 Ovl11EvaluateActionGuardRecord(Ovl11GuardRecord *record);
s32 Ovl11EvaluateActionGuard(s32 kind, u16 payload);
s32 Ovl11TestRecordFieldGuard(s32 index, s32 value);
s32 Ovl11TestRecordRangeGuard(s32 index, s32 selector);
s32 Ovl11TestSelectorRangeGuard(u32 selector);
s32 Ovl11TestStateGuard(s32 selector, s32 limit);
s32 Ovl11IsOutsideGuardInterval(s32 lower, s32 upper);
s32 Ovl11HasActionGuardAreaException(s32 id, s16 area, s16 room);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 Ovl11FindActionGuardResult(s32 id) {
    s16 *record;
    u32 i;

    gOvl11ActionGuardSelector = -1U;
    for (i = 0; i < 31; i++) {
        record = gOvl11ActionGuardLists[i];
        if (record[0] == id) {
            if (Ovl11IsActionGuardAreaBlocked(id) == 1) {
                return 0x1CD;
            }
            return Ovl11EvaluateActionGuardList(record);
        }
    }
    return -1;
}

s32 Ovl11EvaluateActionGuardList(s16 *header) {
    Ovl11GuardRecord *record;
    s16 count;
    s32 i;
    s32 result;

    count = header[1];
    record = *(Ovl11GuardRecord **)((u8 *)header + 4);
    for (i = 0; i < count; i++, record++) {
        result = Ovl11EvaluateActionGuardRecord(record);
        if (result != -1) {
            return result;
        }
    }
    return -1;
}

s32 Ovl11EvaluateActionGuardRecord(Ovl11GuardRecord *record) {
    u8 first;
    u8 second;
    u16 firstValue;
    u16 secondValue;
    s32 firstResult;
    s32 secondResult;
    s32 result;

    first = record->firstKind;
    second = record->secondKind;
    firstValue = record->firstValue;
    secondValue = record->secondValue;
    if (gOvl11ActionGuardSelector == -1U || gOvl11ActionGuardSelector == firstValue) {
        firstResult = Ovl11EvaluateActionGuard(first, firstValue);
        secondResult = Ovl11EvaluateActionGuard(second, secondValue);
        if (firstResult == 1) {
            if (second == 0xFF) {
                return record->result;
            }
            result = -1;
            if (secondResult == firstResult) {
                result = record->result;
            }
            return result;
        }
    }
    return -1;
}

/* Kinds 0/1 split the payload into an index and a selector byte; kinds 2/3
 * consume the whole payload. Only an exact return of 1 satisfies a record. */
s32 Ovl11EvaluateActionGuard(s32 kind, u16 payload) {
    s32 result;

    switch (kind) {
    case 0:
        result = Ovl11TestRecordFieldGuard(payload >> 8, payload & 0xFF);
        break;
    case 1:
        result = Ovl11TestRecordRangeGuard(payload >> 8, payload & 0xFF);
        break;
    case 2:
        result = Ovl11TestSelectorRangeGuard(payload);
        break;
    case 3:
        result = Ovl11TestStateGuard(payload, payload);
        break;
    case 0xFF:
    default:
        result = 0;
        break;
    }
    return result;
}

s32 Ovl11TestRecordFieldGuard(s32 index, s32 value) {
    char *base;
    char *record;

    base = (char *)&D_8006C838;
    record = base + index * 0x1D4;
    return *(s16 *)(record + 0x8000 + 0x1A16) != value;
}

s32 Ovl11TestRecordRangeGuard(s32 index, s32 selector) {
    s32 lower;
    s32 upper;
    s32 firstThreshold;
    s32 secondThreshold;
    s32 value;
    char *record;
    char *base;
    char *otherBase;

    if (Ovl11IsSpecialCounterSlot((s16)index) == 1) {
        base = (char *)&D_8006C838;
        record = base + index * 0x1D4;
        record += 0x8000;
        value = *(u16 *)(record + 0x19EC);
        firstThreshold = 0x2710;
        secondThreshold = 0x7530;
    } else {
        otherBase = (char *)&D_8006C838;
        record = otherBase + index * 0x1D4;
        record += 0x8000;
        value = *(s16 *)(record + 0x19EA);
        firstThreshold = 0x64;
        secondThreshold = 0xC8;
    }
    switch (selector) {
    case 0:
        if (firstThreshold >= value) {
            lower = 0xA;
            upper = 0x12;
        } else {
            lower = 6;
            if (secondThreshold >= value) {
                lower = 8;
                upper = 0x14;
            } else {
                upper = 0x16;
            }
        }
        return Ovl11IsOutsideGuardInterval(lower, upper);
    case 1:
        return firstThreshold >= value;
    case 2:
        return secondThreshold >= value;
    case 3:
        lower = 0x12;
        if (firstThreshold >= value) {
            return 1;
        }
        upper = 0x18;
        return Ovl11IsOutsideGuardInterval(lower, upper);
    case 4:
        return 0;
    default:
        return 0;
    }
}

s32 Ovl11TestSelectorRangeGuard(u32 selector) {
    s32 lower;
    s32 upper;

    switch (selector) {
    case 0:
        lower = 9;
        upper = 0xC;
        break;
    case 1:
    case 3:
        lower = 9;
        upper = 0x10;
        break;
    case 2:
        lower = 9;
        upper = 0x11;
        break;
    case 4:
        lower = 0xD;
        upper = 0x10;
        break;
    case 5:
        lower = 9;
        upper = 0x12;
        break;
    case 6:
        lower = 8;
        upper = 0x15;
        break;
    case 7:
        lower = 0x12;
        upper = 0x18;
        break;
    case 8:
        lower = 0xB;
        upper = 0x11;
        break;
    default:
        return 0;
    }
    return Ovl11IsOutsideGuardInterval(lower, upper);
}

s32 Ovl11TestStateGuard(s32 selector, s32 limit) {
    /* Nesting affects cc1's address expansion in the enclosing function
     * (cse_not_expected). The asm name preserves the callable nested entry,
     * not an instruction injection or a new register/assembly workaround. */
    auto s32 checkAreaFlag(s32 index) __asm__("Ovl11TestAreaFlagGuard");

    s32 flagResult;
    s32 result;
    s32 beforeLimit;

    s32 checkAreaFlag(s32 index) {
        char *base;
        s32 flagResult;

        base = (char *)&D_8006C838;
        if (*(s16 *)(base + 0x44BA) == 3) {
            if (*(s16 *)(base + 0x44BC) == 0x17) {
                flagResult = GetGameFlag(0x1B);
                if (flagResult == 1) {
                    if (GetGameFlag((index + 0x21) & 0xFFFF) == 0) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }

    switch (selector) {
    case 0:
        if (GetGameFlag(0x20U) == 0 && GetGameFlag(0x25U) == 0) {
            result = 0;
        } else {
            result = 1;
        }
        break;
    case 1:
        if (GetGameFlag(0x26U) == 0 && GetGameFlag(0x2AU) == 0) {
            result = 0;
        } else {
            result = 1;
        }
        break;
    case 2:
        result = 0;
        if ((GetGameFlag(0x184U) == 1) && (GetGameFlag(0x185U) == 0)) {
            result = GetGameFlag(0x186U) == 0;
        }
        break;
    case 3:
        result = GetGameFlag(0x27);
        break;
    case 4:
        result = GetGameFlag(0x26);
        break;
    case 5:
        result = gCalendarWeekday == 0;
        break;
    case 6:
        result = GetGameFlag(0x20);
        break;
    case 7: {
        char *base;
        base = (char *)&D_8006C838;
        result = 0;
        if ((*(s16 *) (base + 0x44BA)) == 2) {
            result = (*(s16 *) (base + 0x44BC)) == 4;
        }
        break;
    }
    case 8: {
        s32 value;
        result = 0;
        if (GetGameFlag(0x74U) == 1) {
            value = Ovl11Item_80071A8A.id;
            value -= 0x106;
            result = (u32)value < 2U;
        }
        break;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        result = checkAreaFlag(selector - 9);
        break;
    case 14: {
        char *base;
        base = (char *)&D_8006C838;
        result = 0;
        if ((*(s16 *) (base + 0x44BA)) == 2) {
            result = (*(s16 *) (base + 0x44BC)) == 2;
        }
        break;
    }
    case 15: {
        char *base;
        base = (char *)&D_8006C838;
        result = 0;
        if ((*(s16 *) (base + 0x44BA)) == 0) {
            result = (*(s16 *) (base + 0x44BC)) == 0;
        }
        break;
    }
    case 16:
        result = GetGameFlag(0x1C);
        break;
    case 17:
        result = GetGameFlag(0x1D);
        break;
    case 18:
        result = GetGameFlag(0x1F);
        break;
    case 19: {
        Ovl11GuardState *state;
        Ovl11GuardState *timeState;
        s16 *savedArea;
        s16 *savedRoom;
        result = 0;
        state = (Ovl11GuardState *)D_8006C838;
        savedArea = &state->savedArea;
        if (*savedArea == state->area) {
            state = (Ovl11GuardState *)D_8006C838;
            savedRoom = &state->savedRoom;
            if (*savedRoom == state->room) {
                if ((state->item == 0x103) || (ovl_11_func_800E6EE0(0x103, 1, 1, 0) == 1)) {
                    limit = 0xA;
                } else {
                    limit = 0x12;
                }
                timeState = (Ovl11GuardState *)D_8006C838;
                beforeLimit = timeState->time < limit;
                if (beforeLimit != 0) {
                    result = 1;
                }
            }
        }
        break;
    }
    case 20:
        result = 0;
        flagResult = GetGameFlag(0x19BU);
        if (flagResult == 1) {
            if (gCalendarWeekday != flagResult) {
                result = gCalendarWeekday != 3;
            }
        }
        break;
    case 21:
        result = GetGameFlag(0x2A);
        break;
    default:
        result = 0;
        break;
    }
    if (result == 1) {
        gOvl11ActionGuardSelector = selector;
    }
    return result;
}

/* Half-open [lower, upper); the engine halfword's units are not established
 * here. Return polarity is intentionally outside=1, inside=0. */
s32 Ovl11IsOutsideGuardInterval(s32 lower, s32 upper) {
    if (lower <= D_80070CF8_signedHalfword && D_80070CF8_signedHalfword < upper) {
        return 0;
    }
    return 1;
}

s32 Ovl11IsActionGuardAreaBlocked(s32 id) {
    u8 *base;
    s16 area;
    s16 room;

    base = (u8 *)&D_8006C838;
    area = *(s16 *)(base + 0x44BA);
    room = *(s16 *)(base + 0x44BC);
    if (Ovl11HasActionGuardAreaException(id, area, room) == 1) {
        return 0;
    }
    return Ovl11FindCalendarDate(area, room) != 0;
}

s32 Ovl11HasActionGuardAreaException(s32 id, s16 area, s16 room) {
    Ovl11GuardAreaException *entry;
    Ovl11GuardAreaException **entries;
    u32 i;
    s32 j;

    entries = gOvl11ActionGuardAreaExceptions;
    for (i = 0; i < 3; i++) {
        entry = entries[i];
        if (entry->area == area && entry->room == room) {
            j = 0;
            if (entry->count > 0) {
                do {
                    if (entry->ids[j] == id) {
                        return 1;
                    }
                    j++;
                } while (j < entry->count);
            }
        }
    }
    return 0;
}
