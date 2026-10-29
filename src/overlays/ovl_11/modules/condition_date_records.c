/*
 * Dated-condition record lifecycle: five slots plus the singleton selected by
 * slot -1. Calendar and record-index storage are shared with the EXE; this TU
 * owns the snapshot, assignment, reset and classification protocol, not storage.
 * See ovl11_condition_date_records.h for the externally used operations.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_condition_date_records.h"
#include "game_flags.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_record_flag_counters.h"
#include "ovl11_progress_gate.h"

void Ovl11ClearConditionDateSlot(s16 slot);
s32 Ovl11ConditionDateSlotAccepts(s16 slot, s16 season, s16 day);

void Ovl11ResetConditionDateRecords(void) {
    s16 slot;

    SetGameFlag(0x26U, 0U);
    SetGameFlag(0x27U, 0U);
    SetGameFlag(0x28U, 0U);
    SetGameFlag(0x29U, 0U);
    SetGameFlag(0x2AU, 0U);
    for (slot = 0; slot < 5; slot++) {
        if (Ovl11ClassifyConditionDateSlot(slot) == 1) {
            Ovl11SnapshotConditionDateSlot(slot);
        }
    }
    if (((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BA == 0 &&
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_44BC == 7 &&
        GetGameFlag(0x1BU) == 0) {
        ((struct Ovl11ButtonUpdateState *) D_8006C838)->field_E668 = 0;
    }
}

typedef struct {
    char pad_000[0x44B8];
    u16 year;
    s16 season;
    s16 day;
} ConditionCalendarView;

/* The slot predicate is called but its result is deliberately unused: only
 * the calendar-table search controls the retry. Dates use 0-based days and
 * seasons (30 days, four seasons); the first candidate is seven days later. */
void Ovl11SearchNextConditionDate(s16 slot, Ovl11EF488Record *output) {
    s16 day;
    s16 season;
    s32 year;
    s32 previousYearStep;
    s32 nextYearStepQ16;
    s32 yearsElapsed;

    yearsElapsed = 0;
    season = ((ConditionCalendarView *)D_8006C838)->season;
    day = ((ConditionCalendarView *)D_8006C838)->day;
    if (slot == 4) {
        year = ((ConditionCalendarView *)D_8006C838)->year;
        output->field_2 = 0;
        output->field_4 = 0x1D;
        year += 1;
        output->field_0 = year;
    } else {
        day += 6;
        nextYearStepQ16 = 0x10000;
        do {
            day += 1;
            if (day >= 0x1E) {
                day -= 0x1E;
                season += 1;
                if (season >= 4) {
                    season -= 4;
                    previousYearStep = nextYearStepQ16;
                    nextYearStepQ16 += 0x10000;
                    yearsElapsed = previousYearStep;
                    yearsElapsed = yearsElapsed >> 0x10;
                }
            }
            Ovl11ConditionDateSlotAccepts(slot, season, day);
        } while (Ovl11FindCalendarDateExtended(season, day) != 0);
        year = yearsElapsed + ((ConditionCalendarView *)D_8006C838)->year;
        output->field_2 = season;
        output->field_4 = day;
        output->field_0 = year;
    }
}

s32 Ovl11ClassifyConditionDateSlot(s16 slot) {
    u32 first;
    u32 second;
    s16 *state;
    Ovl11EF488Record *record;

    first = 0;
    second = 0;
    state = (s16 *) D_80070CF0_halfwords;
    if (Ovl11ConditionDateSlotAccepts(slot, state[1], state[2]) == 0) {
        return 0;
    }
    if (slot != -1) {
        record = &gConditionDateSlots[slot];
    } else {
        record = (Ovl11EF488Record *) ((u8 *) state + 0xFD0);
    }
    if (record->index_8 == -1) {
        return 0;
    }
    switch (slot) {
    case -1:
        if (GetGameFlag(0x1B) == 0) {
            return 0;
        }
        /* The singleton shares slot 4's flag and progress tests. */
    case 4:
        if (GetGameFlag(0x122) != 0) {
            return 0;
        }
        if (GetGameFlag(0x2A) == 1) {
            return 1;
        }
        if (Ovl11TestProgressGate(0) == 1) {
            return 0;
        }
        break;
    default:
        switch (slot) {
        case 0:
            first = 0x21;
            second = 0x26;
            break;
        case 1:
            first = 0x22;
            second = 0x27;
            break;
        case 2:
            first = 0x23;
            second = 0x28;
            break;
        case 3:
            first = 0x24;
            second = 0x29;
            break;
        }
        if (GetGameFlag(second) == 1) {
            return 1;
        }
        if (GetGameFlag(first) == 1) {
            return 0;
        }
        break;
    }
    if (slot != 4) {
        if ((s16) record->field_0 > state[0]) {
            return 0;
        }
        if ((s16) record->field_0 == state[0] && (s16) record->field_2 > state[1]) {
            return 0;
        }
        if (record->field_2 == (u16) state[1] && (s16) record->field_4 > state[2]) {
            return 0;
        }
    } else {
        if ((s16) record->field_0 > state[0]) {
            return 0;
        }
        if ((s16) record->field_2 != state[1]) {
            return 0;
        }
        if ((s16) record->field_4 != state[2]) {
            return 0;
        }
    }
    if (slot == -1) {
        return 1;
    }
    if (slot != 4) {
        if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_A * 0x1D4))->field_99EC > 0xEA5F &&
            ((Ovl11EF488SelectionView *) D_8006C838)->selected != -1) {
            return 0;
        }
        if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_8 * 0x1D4))->field_99EA >= 200) {
            return 2;
        }
        if (slot == 0) {
            if (((Ovl11EF488SelectionView *) D_8006C838)->selected == 20) {
                if (GetGameFlag(0x1B) == 0) {
                    return 2;
                } else {
                    return 1;
                }
            }
        }
    } else {
        if (GetGameFlag(0xC9) == 1) {
            if (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + record->index_A * 0x1D4))->field_99EC > 0xEA5F &&
                ((Ovl11EF488SelectionView *) D_8006C838)->selected != -1) {
                return 0;
            }
            return 1;
        }
        if (GetGameFlag(0x11E) == 1) {
            return 2;
        }
    }
    return 1;
}

void Ovl11SnapshotConditionDateSlot(s16 slot) {
    Ovl11EF488Record *record;
    u16 *source;
    u16 first;
    u16 second;

    first = 0;
    second = 0;
    source = D_80070CF0_halfwords;
    switch (slot) {
    case -1:
        Ovl11SelectCounterRecord(((Ovl11EF488SelectionView *) D_8006C838)->selected);
        switch (((Ovl11EF488SelectionView *) D_8006C838)->selected) {
        case 3:
            first = 0x1C;
            break;
        case 7:
            first = 0x1D;
            break;
        case 9:
            first = 0x1E;
            break;
        case 14:
            first = 0x1F;
            break;
        case 20:
        default:
            first = 0x20;
            break;
        }
        break;
    case 0:
        first = 0x21;
        second = 0x26;
        break;
    case 1:
        first = 0x22;
        second = 0x27;
        break;
    case 2:
        first = 0x23;
        second = 0x28;
        break;
    case 3:
        first = 0x24;
        second = 0x29;
        break;
    case 4:
        first = 0x25;
        second = 0x2A;
        break;
    }
    if (slot == -1) {
        record = &gConditionDateSingleton;
    } else {
        record = &gConditionDateSlots[slot];
    }
    record->field_0 = source[0];
    record->field_2 = source[1];
    record->field_4 = source[2];
    record->field_6 = source[3];
    SetGameFlag(first, 1);
    if (slot == -1) {
        SetGameFlag(0x1B, 0);
        SetGameFlag(0x41, 1);
        return;
    }
    SetGameFlag(second, 1);
    ((Ovl11EF488FlagView *) ((u8 *) D_8006C838 +
        ((Ovl11EF488RecordsView *) D_8006C838)->records[slot].index_8 * 0x1D4))->flags |= 8;
    ((Ovl11EF488FlagView *) ((u8 *) D_8006C838 +
        ((Ovl11EF488RecordsView *) D_8006C838)->records[slot].index_A * 0x1D4))->flags |= 8;
}

/* Bank zero assigns one of five dated slots. Nonzero banks use the singleton
 * and clear the corresponding ordinary slot; selector 5 maps to slot 4 and
 * also sets both designated record flags. */
void Ovl11AssignConditionDateSlot(s16 bank, s16 selector) {
    Ovl11EF488Record *record;
    u8 *flagRecord;
    char *base;
    s16 firstIndex;
    s16 selectedBank;
    s16 secondIndex;
    s16 selectedSlot;
    u8 *recordBase;

    selectedBank = bank;
    selectedSlot = selector;
    if (selectedSlot == 5) {
        selectedSlot = 4;
        selectedBank = 0;
        SetGameFlag(0x25U, 1U);
        base = (char *) D_8006C838;
        base += 0x8000;
        (*(u16 *) (base + 0x47BA)) = (u16) ((*(u16 *) (base + 0x47BA)) | 8);
        (*(u16 *) (base + 0x3E96)) = (u16) ((*(u16 *) (base + 0x3E96)) | 8);
    }
    if (selectedBank == 2) {
        firstIndex = 0x29;
        secondIndex = selectedSlot;
    } else {
        switch (selectedSlot) {
        case 0:
            secondIndex = 3;
            firstIndex = 0x13;
            break;
        case 1:
            secondIndex = 7;
            firstIndex = 0xB;
            break;
        case 2:
            secondIndex = 9;
            firstIndex = 8;
            break;
        case 3:
            secondIndex = 0xE;
            firstIndex = 5;
            break;
        case 4:
        default:
            secondIndex = 0x14;
            firstIndex = 0x19;
            break;
        }
    }
    if (selectedBank != 0) {
        record = &gConditionDateSingleton;
        record->index_8 = 0x29;
        record->index_A = secondIndex;
        Ovl11SearchNextConditionDate(-1, record);
        Ovl11ClearConditionDateSlot(selectedSlot);
        recordBase = (u8 *) record - 0x5488;
        flagRecord = recordBase + secondIndex * 0x1D4;
        (*(u16 *) (flagRecord + 0x9A06)) = (u16) ((*(u16 *) (flagRecord + 0x9A06)) | 8);
        return;
    }
    record = &gConditionDateSlots[selectedSlot];
    record->index_8 = firstIndex;
    record->index_A = secondIndex;
    Ovl11SearchNextConditionDate(selectedSlot, record);
}

void Ovl11ClearConditionDateSlot(s16 slot) {
    Ovl11RecordE4D8View *v;

    /* Keep the indexed halfword lvalues: they preserve this reset's address
     * expression family. Halfword 3 intentionally retains its prior value. */
    v = (Ovl11RecordE4D8View *)&D_8006C838;
    v->recs[slot][0] = -1;
    v->recs[slot][1] = 0;
    v->recs[slot][2] = 0;
    v->recs[slot][4] = -1;
    v->recs[slot][5] = -1;
}

typedef struct {
    char pad_0000[0x5246];
    s16 excludedSeason;
    s16 excludedDay;
    char pad_524A[0x5492 - 0x524A];
    s16 selectedIndex;
} ConditionDateExclusionView;

s32 Ovl11ConditionDateSlotAccepts(s16 slot, s16 season, s16 day) {
    char *base;
    char *base0;
    char *base1;
    char *q;
    char *t;
    s16 index;

    index = ((ConditionDateExclusionView *)D_8006C838)->selectedIndex;
    if (season == 3 && day == 0xD) {
        return 0;
    }
    switch (slot) {
    case -1:
        if (season == 0 && day == 0x1D) {
            return 0;
        }
        if (((ConditionDateExclusionView *)D_8006C838)->excludedSeason == season &&
            ((ConditionDateExclusionView *)D_8006C838)->excludedDay == day) {
            return 0;
        }
        base = (char *)&D_8006C838;
        q = base + index * 0x1D4;
        if (*(s16 *)(q + 0x8000 + 0x1A0C) == season &&
            *(s16 *)(q + 0x8000 + 0x1A0E) == day) {
            return 0;
        }
        break;
    case 0:
        base0 = (char *)&D_8006C838;
        t = base0 + 0x8000;
        if (t[0x664E] != 0xFF) {
            return 0;
        }
        break;
    case 1:
        base1 = (char *)&D_8006C838;
        if (season == *(s16 *)(base1 + 0x8000 + 0x26D8) &&
            *(s16 *)(base1 + 0x8000 + 0x26DA) != 0) {
            return 0;
        }
        if (season == 2 && day == 4) {
            return 0;
        }
        break;
    default:
        break;
    }
    return 1;
}
