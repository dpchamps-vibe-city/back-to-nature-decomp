/*
 * Time-of-day color-row presentation for scene-kind 15, excluding id E5.
 * The clock/season classifier and sunset lookup are private to the init/update
 * pair. Capture two 16-pixel RGB555 rows at VRAM x=320 and upload their blend
 * at y=220. Dusk is driven by in-game minutes, capped at 113; after the dusk
 * endpoint a separate 32-step row blend bounces once per update at night.
 * See ovl11_time_of_day_fade.h and notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "color_row_blend.h"
#include "ovl11_time_of_day_fade.h"

s16 Ovl11GetSeasonSunsetHour(s16 season);
s32 Ovl11ClassifyTimeOfDay(s16 hour, s16 sunsetHour);

/* The established view exposes season/hour/minute at +44BA/+44C0/+44C2. */
#define CLOCK_STATE ((struct struct_8006C838_button *)D_8006C838)

void Ovl11InitTimeOfDayFade(void) {
    s16 sunsetHour;
    s16 span;
    s32 timeBand;
    s32 elapsedMinutes;
    s32 sourceRow;

    gOvl11ColorRowProgress = 0;
    sunsetHour = Ovl11GetSeasonSunsetHour(CLOCK_STATE->field_44BA);
    timeBand = Ovl11ClassifyTimeOfDay(CLOCK_STATE->field_44C0, sunsetHour);
    if (timeBand != 2) {
        if (timeBand < 3) {
            span = 0x71;
            if (timeBand < 0) {
                goto disabled;
            }
            sourceRow = 0xDF;
            goto initialize;
        }
        if (timeBand == 3) {
            goto night;
        }
        sourceRow = 0;
        span = 0;
        goto initialize;
    }
    elapsedMinutes = ((CLOCK_STATE->field_44C0 - sunsetHour) * 0x3C) + CLOCK_STATE->field_44C2;
    gOvl11ColorRowProgress = elapsedMinutes;
    if (elapsedMinutes >= 0x71) {
        gOvl11ColorRowProgress = 0x71;
    }
    span = 0x71;
    sourceRow = 0xDF;
    goto initialize;
night:
    span = 0x20;
    sourceRow = 0xDE;
    goto initialize;
disabled:
    sourceRow = 0;
    span = 0;
initialize:
    InitColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowSources, span, 0x140, sourceRow, 0x140, sourceRow - 1, 0x140, 0xDC);
}

void Ovl11UpdateTimeOfDayFade(void) {
    s16 sunsetHour;
    s32 timeBand;
    s32 elapsedMinutes;

    sunsetHour = Ovl11GetSeasonSunsetHour(CLOCK_STATE->field_44BA);
    timeBand = Ovl11ClassifyTimeOfDay(CLOCK_STATE->field_44C0, sunsetHour);
    if (timeBand != 2) {
        if (timeBand >= 3) {
            if (timeBand == 3) {
                goto night;
            }
        } else {
            if (timeBand < 0) {
                return;
            }
            DrawColorRowBlend(&gOvl11ColorRowBlend, 0);
        }
        return;
    }
    if (gOvl11ColorRowProgress < 0x71) {
        /* Draw the old progress, then sample the clock for the next update. */
        DrawColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowProgress);
        elapsedMinutes = (CLOCK_STATE->field_44C0 - sunsetHour) * 0x3C + CLOCK_STATE->field_44C2;
        gOvl11ColorRowProgress = elapsedMinutes;
        if (elapsedMinutes >= 0x71) {
            gOvl11ColorRowProgress = 0x71;
            return;
        }
    } else {
        if (gOvl11ColorRowProgress == 0x71) {
            InitColorRowBlend(&gOvl11ColorRowBlend, gOvl11ColorRowSources, 0x20, 0x140, 0xDE, 0x140, 0xDD, 0x140, 0xDC);
            gOvl11ColorRowProgress += 1;
            return;
        }
        return;
    }
    return;
night:
    StepColorRowBlend(&gOvl11ColorRowBlend, 1, 1);
}

#undef CLOCK_STATE

/* For the supported sunset table: 0=dawn (5..6), 1=day (7..sunset-1),
 * 2=dusk (sunset..sunset+1), 3=night. Retain the original decision tree. */
s32 Ovl11ClassifyTimeOfDay(s16 hour, s16 sunsetHour) {
    s16 duskEndHour;
    s32 beforeSunset;
    s32 timeBand;

    timeBand = 3;
    duskEndHour = sunsetHour + 2;
    if ((u32) (hour - 5) < 2U) {
        timeBand = 0;
    }
    beforeSunset = hour < sunsetHour;
    if (hour >= 7) {
        if (beforeSunset != 0) {
            timeBand = 1;
            goto test_dusk;
        }
        goto before_dusk_end;
    }
test_dusk:
    if (beforeSunset == 0) {
before_dusk_end:
        if (hour < duskEndHour) {
            timeBand = 2;
        }
    }
    return timeBand;
}

s16 Ovl11GetSeasonSunsetHour(s16 season) {
    return gOvl11SunsetHours[season];
}
