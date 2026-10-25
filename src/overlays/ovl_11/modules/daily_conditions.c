/*
 * Calendar-dependent daily-condition selection, ovl_11:800BF8B8..800BFD1C.
 * The chooser prepares the next code; the applier consumes the saved code
 * and publishes the current one. Shared work-state offsets are not private
 * allocations. See ovl11_daily_conditions.h for ordering and code constraints,
 * and notes/file-groupings.md for membership/boundary evidence.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_daily_conditions.h"
#include "ovl11_clock_calendar.h"
#include "random.h"

/* Helpers retain external linkage. Co-location does not establish static. */
void Ovl11AdvanceConditionDate(Ovl11ConditionDate *date);
s32 Ovl11IsConditionExcludedDate(s16 *date);
void Ovl11PublishDayConditionFlags(void);
s32 func_8001AF44(u32 slot);
void func_8001AF70(u16 slot, u16 value);

s16 Ovl11ChooseNextDayCondition(void) {
    Ovl11ConditionDate next;
    Ovl11ConditionDate after;
    s32 random;
    u8 *base;

    base = (u8 *)D_8006C838;
    next.season = *(u16 *)(base + 0x44BA);
    next.day = *(u16 *)(base + 0x44BC);
    Ovl11AdvanceConditionDate(&next);
    *(u16 *)(base + 0xE4CA) = 0;
    after.season = next.season;
    after.day = next.day;
    Ovl11AdvanceConditionDate(&after);
    if (Ovl11FindCalendarDate(next.season, next.day) == 0 &&
        (next.day != *(s16 *)(base + 0x548C) ||
         next.season != *(s16 *)(base + 0x548A)) &&
        Ovl11IsConditionExcludedDate((s16 *)&next) != 1) {
        switch (next.season) {
        case 0:
            random = (s32)Rand(10) + 1;
            if (random % 3 == 0) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 1:
            random = Rand(20);
            if (random == 14) {
                *(u16 *)(base + 0xE4CA) = 3;
            } else if (random == 3) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 2:
            if (next.day != 12 && Rand(5) == 3) {
                *(u16 *)(base + 0xE4CA) = 1;
            }
            break;
        case 3:
            random = Rand(10);
            if (random == 7) {
                *(u16 *)(base + 0xE4CA) = 4;
            } else if (random & 1) {
                *(u16 *)(base + 0xE4CA) = 2;
            }
            break;
        }
        if (Ovl11FindCalendarDate(after.season, after.day) != 0 &&
            (s16)((Ovl11DayConditionWorkView *)D_8006C838)->nextCondition >= 3) {
            ((Ovl11DayConditionWorkView *)D_8006C838)->nextCondition -= 2;
        }
    }
    return (s16)((Ovl11DayConditionWorkView *)D_8006C838)->nextCondition;
}

/* This lookahead intentionally retains the original zero-based rollover,
 * independent of the clock's stored day convention. No year is tracked. */
void Ovl11AdvanceConditionDate(Ovl11ConditionDate *date) {
    if ((date->day + 1) < 0x1E) {
        date->day = (s16)((u16)date->day + 1);
        return;
    }
    date->day = 0;
    if ((date->season + 1) < 4) {
        date->season = (s16)((u16)date->season + 1);
        return;
    }
    date->season = 0;
}

s16 Ovl11ApplyCurrentDayCondition(void) {
    Ovl11ConditionDate next;
    Ovl11ConditionDate current;

    next.season = ((Ovl11DayConditionWorkView *)D_8006C838)->season;
    current.season = ((Ovl11DayConditionWorkView *)D_8006C838)->season;
    next.day = ((Ovl11DayConditionWorkView *)D_8006C838)->day;
    current.day = ((Ovl11DayConditionWorkView *)D_8006C838)->day;
    Ovl11AdvanceConditionDate(&next);
    ((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition = ((Ovl11DayConditionWorkView *)D_8006C838)->nextCondition;
    if (Ovl11FindCalendarDate((s16)((Ovl11DayConditionWorkView *)D_8006C838)->season, (s16)((Ovl11DayConditionWorkView *)D_8006C838)->day) != 0) {
        ((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition = 0;
    }
    if ((Ovl11FindCalendarDate(next.season, next.day) != 0) && ((s16)((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition >= 3)) {
        ((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition -= 2;
    }
    if (func_8001AF44(0x2CU) == 1) {
        ((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition = 0;
    }
    if (Ovl11IsConditionExcludedDate((s16 *)&current) == 1) {
        ((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition = 0;
    }
    Ovl11PublishDayConditionFlags();
    return (s16)((Ovl11DayConditionWorkView *)D_8006C838)->currentCondition;
}

s32 Ovl11IsConditionExcludedDate(s16 *date) {
    struct_80076220 *entry;
    s32 i;

    for (i = 0; i < 5; i++) {
        entry = &D_80076220 + gOvl11ConditionDateRecordIndices[i];
        if (entry->unk24 == date[0]) {
            if (entry->unk26 == date[1]) {
                return 1;
            }
        }
    }
    return 0;
}

void Ovl11PublishDayConditionFlags(void) {
    u16 condition;
    u32 i;
    char *base;

    for (i = 0; i < 5; i++) {
        func_8001AF70((u16)(i + 0x12), 0);
    }
    base = (char *)&D_8006C838;
    base += 0x8000;
    condition = *(u16 *)(base + 0x64C8);
    func_8001AF70((u16)(condition + 0x12), 1);
}

void Ovl11SetCurrentDayCondition(s16 condition) {
    char *base;

    /* Retain the two-stage address: folding +0x8000 into the store offset
     * changes the original address materialization. */
    base = (char *)&D_8006C838;
    base += 0x8000;
    *(s16 *)(base + 0x64C8) = condition;
}
