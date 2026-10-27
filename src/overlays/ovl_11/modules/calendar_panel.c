/*
 * Interactive 30-day calendar (ovl_11:800F9D3C..800FAAAC).
 *
 * Owns the panel state machine, event-message lookup, cursor navigation and
 * all calendar grid drawing. It reads the current date but never advances
 * the clock or changes the saved date. The shared sprite source, text style
 * and FFFF-terminated scratch text make drawing non-reentrant.
 * Membership and original-boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_calendar_panel.h"
#include "sound_commands.h"
#include "screen_fade.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "beveled_panel.h"
#include "ovl11_menu_exit_flags.h"
#include "ovl11_calendar_hooks.h"

/* Engine text/sprite services; their original pointer-as-word ABI is kept. */
u32 func_80017A64(void);
void func_80017A48(u32 style);
void func_80017A38(s16 x, s16 y);
void SetVal8005E2BC(s32 value);
void func_800248B0(s32 ot, s16 x, s16 y);
s16 *func_8001A970(s32 value, s16 *text, s32 digits);
void func_80017B3C(s32 ot, s32 text, s32 x, s32 y);
void func_80024A10(s32 ot, s16 x, s16 y, s16 frame);
u16 *func_8001AA7C(s32 value, u16 *text);
u16 *func_8001AAB8(s32 weekday, u16 *text);

/* Module-local interfaces retain external linkage: co-location alone does
 * not establish that the original functions were static. */
void Ovl11DrawAndProcessCalendarPanel(void);
void Ovl11ProcessCalendarCloseInput(void);
void Ovl11DrawAndMoveCalendarCursor(void);
void Ovl11DrawCalendarPanelDate(Ovl11CalendarPanelDate *date);
void Ovl11DrawCalendarEventMarkers(s32 ot, s16 season, s16 firstWeekday);
void Ovl11DrawCalendarSavedDateMarker(s32 ot, s16 year, s16 season, s16 firstWeekday);
void Ovl11ShowCalendarDateMessage(s32 unusedOt, s32 unusedTextOt, s16 year, s16 season, s16 day);
void Ovl11DrawCalendarCursor(s32 ot, s16 day, s16 firstWeekday);
void Ovl11DrawCalendarCurrentDayMarker(s32 ot, s16 day, s16 firstWeekday);
void Ovl11DrawCalendarEventMarker(s32 ot, s16 day, s16 firstWeekday);
void Ovl11DrawCalendarSavedDayMarker(s32 ot, s16 day, s16 firstWeekday);
void Ovl11DrawCalendarMarker(s32 style, s32 ot, s16 day, s16 firstWeekday);
void Ovl11DrawCalendarHeading(s32 ot, s16 year, s16 season);
void Ovl11DrawCalendarDayNumbers(s32 ot, s16 firstWeekday);
void Ovl11DrawCalendarDayNumber(s32 ot, s16 day, s16 firstWeekday, s16 colorClass);
void Ovl11GetCalendarCellPosition(s16 day, s16 firstWeekday, s16 *x, s16 *y);
void Ovl11DrawCalendarWeekdayHeadings(u32 *panelOt, s32 textOt);

void Ovl11ResetCalendarPanel(void) {
    gOvl11CalendarPanelState = 0;
    gOvl11CalendarSelectedDay = 0;
    gOvl11CalendarCloseHookEnabled = 1;
}

void Ovl11OpenCalendarPanel(void) {
    s32 closeHookEnabled;

    Ovl11ResetCalendarPanel();
    closeHookEnabled = Ovl11CalendarCloseHookEnabled();
    gOvl11CalendarCloseHookEnabled = closeHookEnabled;
    Ovl11CalendarOpenHook();
    PlaySoundEffect(3);
    gOvl11CalendarPanelState = 1;
    gOvl11CalendarSelectedDay = gCalendarDay;
}

void Ovl11UpdateCalendarPanel(void) {
    u32 previousStyle;

    if (gOvl11CalendarPanelState != 0) {
        DrawScreenSubtractOverlay(0x40000, 0x40000, 0x40000);
        if (gOvl11CalendarPanelState == 2) {
            if (GetMessageWindowStatus() == 2) {
                ResetMessageWindow();
                gOvl11CalendarPanelState = 0;
                Ovl11PostMenuExitFlags();
            }
        } else {
            previousStyle = func_80017A64();
            func_80017A48(3);
            Ovl11DrawAndProcessCalendarPanel();
            func_80017A48(previousStyle);
        }
    }
}

void Ovl11DrawAndProcessCalendarPanel(void) {
    struct Ovl11B264StateView *dateWork;
    Ovl11CalendarPanelDate *date;
    s32 drawContext;

    if (IsMessageWindowStatusFinal() && (((GfxObj *)gPad0Input)->field_8 & 0x800)) {
        if (gOvl11CalendarCloseHookEnabled == 1) {
            Ovl11CalendarCloseHook();
        }
        CloseMessageWindow();
        gOvl11CalendarPanelState = 2;
        PlaySoundEffect(3);
    } else {
        date = (Ovl11CalendarPanelDate *)D_80070CF0_halfwords;
        Ovl11DrawCalendarPanelDate(date);
        Ovl11DrawAndMoveCalendarCursor();
        if (gOvl11CalendarMessageState == 0xFF) {
            SetMessagePortraitEnabled(0);
            SetVal8005E2BC(0);
            RequestMessageWindow(3, 0x3B4);
        } else {
            drawContext = gActiveRenderContext->field_D8;
            /* Keep the work-area-relative date view used by this call path;
             * direct field loads introduce an extra instruction here. */
            dateWork = (struct Ovl11B264StateView *)((char *)date - 0x44B8);
            Ovl11ShowCalendarDateMessage(drawContext + 0x58, drawContext + 0x5C,
                dateWork->field_44B8, dateWork->field_44BA,
                (s16)gOvl11CalendarSelectedDay);
            return;
        }
        Ovl11ProcessCalendarCloseInput();
    }
}

void Ovl11ProcessCalendarCloseInput(void) {
    s32 buttons;

    if (IsMessageWindowStatusFinal() != 0) {
        buttons = ((GfxObj *)gPad0Input)->field_8;
        if (buttons & 0x50) {
            CloseMessageWindow();
            PlaySoundEffect(0);
            gOvl11CalendarMessageState = 0;
            return;
        }
        if (buttons & 0x20) {
            if (gOvl11CalendarCloseHookEnabled == 1) {
                Ovl11CalendarCloseHook();
            }
            CloseMessageWindow();
            gOvl11CalendarPanelState = 2;
            PlaySoundEffect(3);
        }
    }
}

void Ovl11DrawAndMoveCalendarCursor(void) {
    s32 buttons;
    s32 weekdayBase;
    s16 currentDay;

    if ((gOvl11CalendarMessageState == 0xFF) && (IsMessageWindowStatusFinal() != 0)) {
        if (*(s32 *)gPad0Input & 0xF000) {
            PlaySoundEffect(5);
            buttons = *(s32 *)gPad0Input;
            /* Vertical moves wrap by complete weeks, adjusting the two short
             * edge rows of the 30-day month. Do not replace with modulo 30. */
            if (buttons & 0x1000) {
                gOvl11CalendarSelectedDay -= 7;
                if (gOvl11CalendarSelectedDay < 0) {
                    gOvl11CalendarSelectedDay += 28;
                    if (gOvl11CalendarSelectedDay < 23) {
                        gOvl11CalendarSelectedDay += 7;
                    }
                }
            } else if (buttons & 0x4000) {
                gOvl11CalendarSelectedDay += 7;
                if (gOvl11CalendarSelectedDay >= 30) {
                    gOvl11CalendarSelectedDay -= 28;
                    if (gOvl11CalendarSelectedDay >= 7) {
                        gOvl11CalendarSelectedDay -= 7;
                    }
                }
            } else if (buttons & 0x8000) {
                --gOvl11CalendarSelectedDay;
                if (gOvl11CalendarSelectedDay < 0) {
                    gOvl11CalendarSelectedDay += 30;
                }
            } else {
                ++gOvl11CalendarSelectedDay;
                if (gOvl11CalendarSelectedDay >= 30) {
                    gOvl11CalendarSelectedDay -= 30;
                }
            }
        }
    }
    currentDay = ((struct Ovl11MenuDateView *)D_8006C838)->field_44BC;
    weekdayBase = (s16)(currentDay % 7);
    weekdayBase -= 7;
    Ovl11DrawCalendarCursor(gActiveRenderContext->field_D8 + 0x50,
        (s16)gOvl11CalendarSelectedDay,
        (s16)((((struct Ovl11MenuDateView *)D_8006C838)->field_44BE - weekdayBase) % 7));
}

void Ovl11DrawCalendarPanelDate(Ovl11CalendarPanelDate *date) {
    s16 firstWeekday;
    s32 weekdayBase;
    s32 currentContext;
    s32 drawContext;
    s32 markerOt;
    s32 textOt;

    weekdayBase = (s16)((s16)date->day % 7);
    weekdayBase -= 7;
    firstWeekday = (date->weekday - weekdayBase) % 7;
    drawContext = gActiveRenderContext->field_D8;
    textOt = drawContext + 0x60;
    markerOt = drawContext + 0x54;
    func_80017A38(1, 2);
    currentContext = gActiveRenderContext->field_D8;
    Ovl11DrawCalendarWeekdayHeadings((u32 *)(currentContext + 0x64), currentContext + 0x60);
    Ovl11DrawCalendarHeading(textOt, date->year, date->season);
    Ovl11DrawCalendarDayNumbers(textOt, firstWeekday);
    Ovl11DrawCalendarCurrentDayMarker(markerOt, (s16)date->day, firstWeekday);
    Ovl11DrawCalendarEventMarkers(markerOt, date->season, firstWeekday);
    Ovl11DrawCalendarSavedDateMarker(markerOt, date->year, date->season, firstWeekday);
    func_80017A38(0, 0);
}

void Ovl11DrawCalendarEventMarkers(s32 ot, s16 season, s16 firstWeekday) {
    Ovl11CalendarPanelEvent *event;
    s32 remaining;

    event = &gOvl11CalendarPanelEvents;
    remaining = 0x11;
    do {
        if (event->season == season) {
            Ovl11DrawCalendarEventMarker(ot, event->day, firstWeekday);
        }
        remaining -= 1;
        event += 1;
    } while (remaining >= 0);
}

void Ovl11DrawCalendarSavedDateMarker(s32 ot, s16 year, s16 season, s16 firstWeekday) {
    Ovl11CalendarSavedDateView *saved = (Ovl11CalendarSavedDateView *)&D_8006C838;

    if (saved->index != -1) {
        if (saved->year == year && saved->season == season) {
            Ovl11DrawCalendarSavedDayMarker(ot, saved->day, firstWeekday);
        }
    }
}

void Ovl11ShowCalendarDateMessage(s32 unusedOt, s32 unusedTextOt, s16 year, s16 season, s16 day) {
    Ovl11CalendarPanelEvent *event;
    s32 remaining;
    s16 message;

    if (gOvl11CalendarMessageState == 0xFF) {
        return;
    }
    message = 0x3DC;
    event = &gOvl11CalendarPanelEvents;
    remaining = 0x11;
    do {
        if (event->season == season && event->day == day) {
            message = event->message;
        }
        remaining -= 1;
        event += 1;
    } while (remaining >= 0);
    /* Later matching events win; the saved date takes precedence over all
     * table entries, even when its marker is suppressed by index == -1. */
    if (((Ovl11CalendarSavedDateView *)D_8006C838)->year == year &&
        ((Ovl11CalendarSavedDateView *)D_8006C838)->season == season &&
        ((Ovl11CalendarSavedDateView *)D_8006C838)->day == day) {
        message = 0x3DD;
    }
    RequestMessageWindow(3, message);
    if (GetMessageWindowStatus() == 2) {
        gOvl11CalendarMessageState = 0xFF;
        CloseMessageWindow();
    }
}

void Ovl11DrawCalendarCursor(s32 ot, s16 day, s16 firstWeekday) {
    s16 x;
    s16 y;

    Ovl11GetCalendarCellPosition(day, firstWeekday, &x, &y);
    func_800248B0(ot, (s16)(x + 0xC), (s16)(y + 0x9));
}

void Ovl11DrawCalendarCurrentDayMarker(s32 ot, s16 day, s16 firstWeekday) {
    Ovl11DrawCalendarMarker(0, ot, day, firstWeekday);
}

void Ovl11DrawCalendarEventMarker(s32 ot, s16 day, s16 firstWeekday) {
    Ovl11DrawCalendarMarker(1, ot, day, firstWeekday);
}

void Ovl11DrawCalendarSavedDayMarker(s32 ot, s16 day, s16 firstWeekday) {
    Ovl11DrawCalendarMarker(2, ot, day, firstWeekday);
}

void Ovl11DrawCalendarMarker(s32 style, s32 ot, s16 day, s16 firstWeekday) {
    s16 x;
    s16 y;
    Ovl11CalendarMarkerStyle *marker;

    marker = &gOvl11CalendarMarkerStyles[style];
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, marker->animation);
    SelectSpriteAnimationFrame((ObjectState *)&gSharedSpriteSource, marker->frame);
    Ovl11GetCalendarCellPosition(day, firstWeekday, &x, &y);
    x = (u16)x + marker->xOffset;
    DrawSpriteFrameRects(ot, (s32)&gSharedSpriteSource,
        (s32)(*(u8 *)((u8 *)&gSharedSpriteSource + 4)),
        (s32)(*(u8 *)((u8 *)&gSharedSpriteSource + 5)), x, y);
}

void Ovl11DrawCalendarHeading(s32 ot, s16 year, s16 season) {
    s16 boundedSeason;
    s16 selectedSeason;

    selectedSeason = season;
    *(u16 *)func_8001A970(year + 1, &gOvl11CalendarNumberText, 2) = 0xFFFF;
    func_80017B3C(ot, (s32)&gOvl11CalendarNumberText, 0x1F, 0x34);
    func_80017B3C(ot, (s32)((void *)((u8 *)&D_80051B9A + *gTextBankOffsets)), 0x39, 0x34);
    if (selectedSeason >= 0) {
        boundedSeason = (selectedSeason < 4) ? selectedSeason : 3;
    } else {
        boundedSeason = 0;
    }
    selectedSeason = boundedSeason;
    func_80024A10(ot, 0x8E, 0x34, selectedSeason);
    *func_8001AA7C((s32)selectedSeason, (u16 *)&gOvl11CalendarNumberText) = 0xFFFF;
    func_80017B3C(ot, (s32)&gOvl11CalendarNumberText, 0x9E, 0x34);
}

void Ovl11DrawCalendarDayNumbers(s32 ot, s16 firstWeekday) {
    s16 colorClass;
    s32 day;

    for (day = 0; day < 0x1E; day++) {
        colorClass = 2;
        if ((firstWeekday + day) % 7 != 0) {
            colorClass = (firstWeekday + day) % 7 == 6;
        }
        Ovl11DrawCalendarDayNumber(ot, day, firstWeekday, colorClass);
    }
}

void Ovl11DrawCalendarDayNumber(s32 ot, s16 day, s16 firstWeekday, s16 colorClass) {
    s16 x;
    s16 y;
    u32 previousStyle;
    u32 style;

    *((u16 *)func_8001A970(day + 1, &gOvl11CalendarNumberText, 2)) = 0xFFFF;
    Ovl11GetCalendarCellPosition(day, firstWeekday, &x, &y);
    previousStyle = func_80017A64();
    style = 5;
    if (colorClass != 1) {
        style = previousStyle;
        if (colorClass == 2) {
            style = 4;
        }
    }
    func_80017A48(style);
    func_80017B3C(ot, (s32)&gOvl11CalendarNumberText, (s32)x, (s32)y);
    func_80017A48(previousStyle);
}

/* Seven columns 41 pixels apart, rows 14 pixels apart, origin (30,80). */
void Ovl11GetCalendarCellPosition(s16 day, s16 firstWeekday, s16 *x, s16 *y) {
    *x = (((day + firstWeekday) % 7) * 0x29) + 0x1E;
    *y = (((day + firstWeekday) / 7) * 0xE) + 0x50;
}

void Ovl11DrawCalendarWeekdayHeadings(u32 *panelOt, s32 textOt) {
    s32 weekday;
    s16 x;
    u32 previousStyle;
    u32 style;

    DrawMenuPanel(panelOt, 1, 0x10, 0x30, 0x11F, 0x74);
    weekday = 0;
    previousStyle = func_80017A64();
    x = 0x16;
    do {
        *func_8001AAB8(weekday, (u16 *)&gOvl11CalendarNumberText) = 0xFFFF;
        if (weekday != 0) {
            style = 5;
            if (weekday < 6) {
                style = previousStyle;
            }
        } else {
            style = 4;
        }
        weekday += 1;
        func_80017A48(style);
        func_80017B3C(textOt, (s32)&gOvl11CalendarNumberText, x, 0x42);
        x += 0x29;
    } while (weekday < 7);
    func_80017A48(previousStyle);
}
