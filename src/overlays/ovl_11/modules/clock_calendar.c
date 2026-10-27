/*
 * Clock/calendar service, ovl_11:800C0688..800C14A0.
 * Batched minute advance and ordinary ticking share one event queue. Calendar
 * flags are refreshed from the same date that drives the event rules.
 * Public contracts: ovl11_clock_calendar.h. Membership/boundary evidence:
 * notes/file-groupings.md. No globals here acquire private linkage.
 */
#include "ovl11_clock_calendar.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "screen_fade.h"
#include "game_flags.h"
#include "ovl11_weather_effect.h"
#include "psyq/abs.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_events.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_event_schedule.h"
#include "ovl11_action_minute_counter.h"

s32 ovl_11_func_800E9B68(s16 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800DE4C0(void);
void ovl_11_func_800CF36C(s32 arg0);
void ovl_11_func_800F0FB0(s32 arg0);

void Ovl11TickClock(void);
void Ovl11ServiceClockEvent(void);
s32 Ovl11IsSceneClockBlocked(void);
void Ovl11RefreshCalendarDayFlags(s16 weekday);
void Ovl11RefreshCalendarSeasonFlags(s32 season);
void Ovl11MatchClockEvent(s16 *rule, u32 firstTest);
void Ovl11QueueClockEvent(s32 eventId);

void Ovl11UpdateClockAndEvents(void) {
    Ovl11TickClock();
    Ovl11ServiceClockEvent();
}

void Ovl11RunClockAdvance(void) {
    s32 index;
    struct Ovl11ClockAnchor *anchor;

    if (gOvl11ClockAdvanceActive != 0) {
        gOvl11ClockAdvanceTickEnabled = 0;
        if (Ovl11HasQueuedClockEvent() == 0) {
            anchor = (struct Ovl11ClockAnchor *)&gMessageWindowPhase;
            while (gOvl11ClockAdvanceMinutesRemaining > 0) {
                anchor->counter = gOvl11ClockMinutePeriodTwice;
                gOvl11ClockAdvanceTickEnabled = 1;
                Ovl11UpdateClockAndEvents();
                gOvl11ClockAdvanceTickEnabled = 0;
                gOvl11ClockAdvanceMinutesRemaining -= 1;
                if (Ovl11HasQueuedClockEvent() != 0) {
                    return;
                }
            }
            anchor->counter = 0;
            gOvl11ClockAdvanceActive = 0;
            gOvl11ClockAdvanceTickEnabled = 1;
            if (((struct Ovl11StateInitView *)D_8006C838)->hour < 6) {
                for (index = 0; index < 0x25; index++) {
                    ovl_11_func_800E9B68((s16)index, 0, 0, 0);
                }
            } else {
                Ovl11SelectScheduleEntryForTime(((struct Ovl11StateInitView *)D_8006C838)->field_44CA);
                Ovl11RebuildObjectEventsForTime();
            }
            if (gOvl11ClockAdvanceRefreshRequested == 1) {
                ovl_11_func_800DE4C0();
                gOvl11ClockAdvanceRefreshRequested = 0;
            }
            Ovl11SelectCurrentHourTint();
        }
    }
}

s32 Ovl11IsClockAdvanceActive(void) {
    return gOvl11ClockAdvanceActive;
}

s32 Ovl11ClassifyClockHour(s16 hour) {
    s32 category;

    category = 3;
    if ((u32)(hour - 5) < 2U) {
        category = 0;
    }
    if ((u32)(hour - 7) < 8U) {
        category = 1;
    }
    if ((u32)(hour - 0xF) < 2U) {
        category = 2;
    }
    return category;
}

s32 Ovl11RequestClockAdvanceToSix(s32 offsetMinutes) {
    char *state;
    s32 hourMinutes;
    s32 minutes;
    s32 hoursBeforeLast;

    state = (char *)&D_8006C838;
    hoursBeforeLast = 5 - *(s16 *)(state + 0x44C0);
    if (hoursBeforeLast < 0) {
        hoursBeforeLast += 0x18;
    }
    gOvl11ClockAdvanceRefreshRequested = 0;
    minutes = 0x3C - *(s16 *)(state + 0x44C2);
    hourMinutes = hoursBeforeLast * 0x3C + offsetMinutes;
    minutes += hourMinutes;
    Ovl11SetClockAdvanceMinutes(minutes);
    return minutes;
}

void Ovl11RequestClockAdvance(s32 minutes) {
    gOvl11ClockAdvanceRefreshRequested = 0;
    Ovl11SetClockAdvanceMinutes(minutes);
}

void Ovl11InitClockAndCalendar(void) {
    struct Ovl11StateInitView *state = (struct Ovl11StateInitView *)D_8006C838;

    state->day = 1;
    state->weekday = 1;
    state->year = 0;
    state->field_44CA = 0x168;
    state->season = 0;
    state->minute = 0;
    state->field_44C8 = 0;
    state->field_D4 = 0;
    state->hour = 6;
    memset(gOvl11ClockEventQueue, -1, 0xC8);
    state->field_D0 = 0;
    gOvl11ClockAdvanceTickEnabled = 0;
    gOvl11ClockAdvanceActive = 0;
    gOvl11ClockAdvanceMinutesRemaining = 0;
    Ovl11RefreshCalendarDayFlags(state->weekday);
    Ovl11RefreshCalendarSeasonFlags(state->season);
    if (Ovl11FindCalendarDate(state->season, state->day) != 0) {
        SetGameFlag(0x2B, 1);
    }
    SetGameFlag(0x96, 1);
}

void Ovl11ResetClockEvents(void) {
    s32 *state = (s32 *)D_8006C838;

    state[0x35] = 0;
    memset(gOvl11ClockEventQueue, -1, 0xC8);
    *(s16 *)((char *)D_8006C838 + 0xD0) = 0;
    gOvl11ClockAdvanceTickEnabled = 0;
    gOvl11ClockAdvanceActive = 0;
    gOvl11ClockAdvanceMinutesRemaining = 0;
}

s32 Ovl11HasQueuedClockEvent(void) {
    return ~gOvl11ClockEventQueue[0] != 0;
}

void Ovl11SetClockAdvanceRefresh(s32 requested) {
    gOvl11ClockAdvanceRefreshRequested = requested;
}

#define CLOCK_STATE ((struct Ovl11ClockState *)D_8006C838)

void Ovl11TickClock(void) {
    s16 *rule;
    s32 periodTwice;
    s32 rolloverFlags;
    u32 ruleIndex;
    struct Ovl11ClockAnchor *anchor;
    struct Ovl11ClockState *state;
    struct Ovl11ClockDate *date;
    struct Ovl11ClockPair *time;
    struct Ovl11ClockPair *dayTime;

    CLOCK_STATE->flags &= 0xBDEFFFFF;
    if (gOvl11ClockAdvanceActive == 0) {
        if (IsScreenFadeComplete() == 0 || GetGameFlag(3U) == 1 ||
            (CLOCK_STATE->flags & 0x08000000) || Ovl11IsSceneClockBlocked() == 1) {
            return;
        }
    } else if (gOvl11ClockAdvanceTickEnabled == 0) {
        return;
    }
    {
        anchor = (struct Ovl11ClockAnchor *)&gMessageWindowPhase;
        state = (struct Ovl11ClockState *)((u8 *)anchor - 0xCC);
        date = (struct Ovl11ClockDate *)((u8 *)anchor + 0x43EC);
        time = (struct Ovl11ClockPair *)((u8 *)anchor + 0x43F4);
        dayTime = (struct Ovl11ClockPair *)((u8 *)anchor + 0x43F8);
        periodTwice = gOvl11ClockMinutePeriodTwice;
        state->flags |= 0x40000000;
        anchor->counter++;
        if (state->flags_10 & 0x01000000) {
            periodTwice = periodTwice / 8 + 1;
        }
        rolloverFlags = 0x8000;
        if (anchor->counter >= (u32)(periodTwice / 2)) {
            anchor->counter = 0;
            time->minute++;
            dayTime->minute++;
            CLOCK_STATE->elapsed++;
            CLOCK_STATE->flags |= 0x100000;
            if (gOvl11ClockAdvanceActive == 0) {
                Ovl11UpdateActionMinuteCounter((u8 *)D_8006C838 + 0x51C8);
            }
            CLOCK_STATE->flags &= 0xFDFFFFFF;
            if (time->minute >= 60) {
                rolloverFlags = 0xC000;
                time->minute = 0;
                dayTime->minute = 0;
                CLOCK_STATE->flags |= 0x02000000;
                time->hour++;
                dayTime->hour++;
            }
            if (time->hour >= 24) {
                CLOCK_STATE->elapsed = 0;
                time->hour = 0;
            }
            /* The date advances only on an hour rollover arriving at six. */
            if (time->hour == 6 && (rolloverFlags & 0x4000)) {
                dayTime->hour = 0;
                date->day++;
                date->weekday++;
                if (date->weekday >= 7) {
                    date->weekday = 0;
                }
            }
            if (date->day >= 30) {
                date->day = 0;
                date->season++;
                ovl_11_func_800CF36C(1);
            }
            if (date->season >= 4) {
                date->season = 0;
                date->year++;
                SetGameFlag(0x96, 0);
                if (date->year >= 99) {
                    date->year = 98;
                }
            }
            Ovl11RefreshCalendarDayFlags(CLOCK_STATE->weekday);
            SetGameFlag(0x2B, 0);
            SetGameFlag(0x2C, 0);
            Ovl11RefreshCalendarSeasonFlags(date->season);
            if (Ovl11FindCalendarDate(CLOCK_STATE->season, CLOCK_STATE->day) != 0) {
                SetGameFlag(0x2B, 1);
            }
            ruleIndex = 0;
            rule = gOvl11ClockEventRules[0];
            do {
                Ovl11MatchClockEvent(rule, 0);
                ruleIndex++;
                rule += 7;
            } while (ruleIndex < 10);
            ovl_11_func_800F0FB0(1);
        }
    }
}

void Ovl11SetClockAdvanceMinutes(s32 minutes) {
    gOvl11ClockAdvanceActive = 1;
    gOvl11ClockAdvanceMinutesRemaining = minutes;
}

/* Seven-halfword rules: year, season, day, unused slot, hour, minute, ID.
 * -1 is a wildcard. Entering at a later test deliberately skips earlier
 * constraints; other firstTest values enqueue unconditionally. */
void Ovl11MatchClockEvent(s16 *rule, u32 firstTest) {
    s16 minute;
    s16 hour;
    s16 day;
    s16 season;
    s16 year;

    switch (firstTest) {
    case 0:
        minute = rule[5];
        if ((minute == -1) || (minute == gClockMinute)) {
        case 1:
            hour = rule[4];
            if ((hour == -1) || (hour == gClockHour)) {
            case 2:
                day = rule[2];
                if ((day == -1) || (day == gCalendarDay)) {
                case 3:
                    season = rule[1];
                    if ((season == -1) || (season == gCalendarSeason)) {
                    case 4:
                        year = rule[0];
                        if ((year == -1) || (year == gCalendarYear)) {
                        default:
                            Ovl11QueueClockEvent((s32)rule[6]);
                        }
                    }
                }
            }
        }
        break;
    }
}

/* First free slot wins; a full queue silently drops the request. IDs and
 * duplicates are not checked. Producers must supply valid callback indices. */
void Ovl11QueueClockEvent(s32 eventId) {
    s32 slot;

    slot = 0;
    if (gOvl11ClockEventQueue[0] == -1) {
        gOvl11ClockEventQueue[0] = eventId;
        return;
    }
nextSlot:
    slot++;
    if (slot >= 0x32) {
        goto done;
    }
    if (gOvl11ClockEventQueue[slot] == -1) {
        gOvl11ClockEventQueue[slot] = eventId;
        goto done;
    }
    goto nextSlot;
done:
    return;
}

void Ovl11ServiceClockEvent(void) {
    s32 slot;

    if (gOvl11ClockEventQueue[0] == -1) {
        return;
    }
    if ((*gOvl11ClockEventHandlers[gOvl11ClockEventQueue[0]])() != 0) {
        D_8006C908 = 0;
        for (slot = 1; slot < 0x32; slot++) {
            s32 eventId = gOvl11ClockEventQueue[slot];
            gOvl11ClockEventQueue[slot - 1] = eventId;
        }
        slot += -1;
        gOvl11ClockEventQueue[slot] = -1;
    }
}

s32 Ovl11IsSceneClockBlocked(void) {
    char *work = (char *)&D_8007AFF0;
    char *scene = *(char **)(work + 0x25388);
    s16 index = *(s16 *)(scene + 2);

    return D_80125F24[index] == 0x1000;
}

#define CALENDAR_STATE ((struct Ovl11FlagDateView *)D_8006C838)

void Ovl11RefreshCalendarDayFlags(s16 weekday) {
    u32 flag;
    s16 *savedDate;
    struct Ovl11FlagDatePair *lastDate;

    for (flag = 0; flag < 7; flag++) {
        SetGameFlag((u16)(flag + 0xB), 0);
    }
    SetGameFlag((u16)(weekday + 0xB), 1);
    for (flag = 0x78; flag < 0x96; flag++) {
        SetGameFlag((u16)flag, 0);
    }
    SetGameFlag((u16)(CALENDAR_STATE->field_44BC + 0x78), 1);
    SetGameFlag(0x97, 0);
    if (CALENDAR_STATE->field_44BA == CALENDAR_STATE->field_5246 && CALENDAR_STATE->field_44BC == CALENDAR_STATE->field_5248) {
        SetGameFlag(0x97, 1);
    }
    if (GetGameFlag(0x1B) != 1) {
        SetGameFlag(0x99, 0);
        if (CALENDAR_STATE->field_44BA == CALENDAR_STATE->field_E798) {
            if (CALENDAR_STATE->field_44BC == CALENDAR_STATE->field_E79A) {
                SetGameFlag(0x99, 1);
            }
        }
        SetGameFlag(0x98, 0);
        savedDate = (s16 *)((u8 *)D_80076244 + CALENDAR_STATE->field_E778 * 0x1D4);
        if (CALENDAR_STATE->field_44BA == savedDate[0] && CALENDAR_STATE->field_44BC == savedDate[1]) {
            SetGameFlag(0x98, 1);
        }
        if (GetGameFlag(0x4F) != 1) {
            SetGameFlag(0x9A, 0);
            lastDate = &D_8007A414;
            if (CALENDAR_STATE->field_44BA == lastDate->field_0 && CALENDAR_STATE->field_44BC == lastDate->field_2) {
                SetGameFlag(0x9A, 1);
            }
        }
    }
}

void Ovl11RefreshCalendarSeasonFlags(s32 season) {
    s16 selected;
    u32 flag;

    selected = season;
    for (flag = 0; flag < 4; flag++) {
        SetGameFlag((u16)(flag + 0x17), 0);
    }
    SetGameFlag((u16)(selected + 0x17), 1);
}

s32 Ovl11FindCalendarDate(s16 season, s16 day) {
    s32 row;

    for (row = 0; row < 14; row++) {
        if (season == ((Ovl11CalendarDateEntry *)gOvl11CalendarDates)[row].season && day == ((Ovl11CalendarDateEntry *)gOvl11CalendarDates)[row].day) {
            return row + 1;
        }
    }
    return 0;
}

s32 Ovl11FindCalendarDateExtended(s16 season, s16 day) {
    u32 row;

    for (row = 0; row < 18; row++) {
        if (season == ((Ovl11CalendarDateEntry *)gOvl11CalendarDates)[row].season && day == ((Ovl11CalendarDateEntry *)gOvl11CalendarDates)[row].day) {
            return row + 1;
        }
    }
    return 0;
}

void Ovl11RefreshSavedDateFlags(void) {
    Ovl11Rec64D8View *records;
    u16 flag;
    u32 index;

    index = 0;
    records = (Ovl11Rec64D8View *)D_80074838;
    for (; index < 5U; index++) {
        flag = (index + 0x26) & 0xFFFF;
        SetGameFlag(flag, 0U);
        if ((((Ovl11Status44B8View *)D_8006C838)->field_44B8 == records->recs[index][0]) && (((Ovl11Status44B8View *)D_8006C838)->field_44BA == records->recs[index][1]) && (((Ovl11Status44B8View *)D_8006C838)->field_44BC == ((Ovl11Status44B8View *)D_8006C838)->field_E4DC[index * 6])) {
            SetGameFlag(0x2CU, 1U);
            SetGameFlag((index + 0x21) & 0xFFFF, 1U);
            SetGameFlag(flag, 1U);
        }
    }
    if ((((Ovl11Status44B8View *)D_8006C838)->field_44B8 == ((Ovl11Status44B8View *)D_8006C838)->field_5488) && (((Ovl11Status44B8View *)D_8006C838)->field_44BA == ((Ovl11Status44B8View *)D_8006C838)->field_548A) && (((Ovl11Status44B8View *)D_8006C838)->field_44BC == ((Ovl11Status44B8View *)D_8006C838)->field_548C)) {
        SetGameFlag(0x2CU, 1U);
    }
}

s16 Ovl11RemapCalendarValue(s16 value) {
    u8 *state;
    s16 result;
    s32 dateIndex;
    s16 replacement;

    result = value;
    if ((u32)(result - 0x34) < 4U) {
        state = (u8 *)&D_8006C838;
        dateIndex = Ovl11FindCalendarDate(*(s16 *)(state + 0x44BA), *(s16 *)(state + 0x44BC));
        if (dateIndex != 0) {
            /* Keep the flat halfword view: the searchers use six-byte records,
             * while this consumer indexes their third halfword explicitly. */
            replacement = gOvl11CalendarDates[(dateIndex - 1) * 3 + 2];
            if (replacement != -1) {
                result = replacement;
            }
        }
    }
    return result;
}
