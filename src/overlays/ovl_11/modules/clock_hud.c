/*
 * Compact season/date/clock HUD (ovl_11:800FB5FC..800FB908).
 * Four visibility controls feed one renderer and one private text buffer.
 * Date and clock state are read only; no ticking or calendar-panel state.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_clock_hud.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "beveled_panel.h"

void func_80024A10(s32 ot, s16 x, s16 y, s16 frame);
s16 *func_8001A970(s32 value, s16 *text, s32 digits);
void func_80017B3C(s32 ot, s32 text, s32 x, s32 y);
u16 *func_8001AAB8(s32 weekday, u16 *text);
u32 func_80017A64(void);
void func_80017A48(u32 style);
void func_80017D9C(s32 *ot, u16 *text, s16 x, s16 y, s32 frame);
void Ovl11DrawClockHud(void);

void Ovl11ResetClockHud(void) {
    gOvl11ClockHudEnabled = 0;
}

void Ovl11EnableClockHud(void) {
    if (gOvl11ClockHudEnabled == 0) {
        gOvl11ClockHudEnabled = 1;
    }
}

void Ovl11DisableClockHud(void) {
    if (gOvl11ClockHudEnabled != 0) {
        gOvl11ClockHudEnabled = 0;
    }
}

void Ovl11UpdateClockHud(void) {
    if (gOvl11ClockHudEnabled != 0) {
        Ovl11DrawClockHud();
    }
}

void Ovl11DrawClockHud(void) {
    s16 x, y;
    s32 repeat;
    s16 *cursor;
    s16 *display, *finalDisplay, *glyphDisplay;
    s16 row, minuteTens;
    s32 hour;
    s32 fiveMinuteDigit, minute;
    s32 wideMinuteTens;
    s32 textOt;
    s32 beforeNoon;
    s32 drawContext;
    u32 previousStyle;
    u32 style;

    /* Keep the single-pass scopes and separate cursor lifetimes: formatted
     * time text and its final glyph share storage across two draw services. */
    repeat = 0;
    drawContext = gActiveRenderContext->field_D8;
    textOt = drawContext + 0x8C;
    beforeNoon = (((struct Ovl11CalendarDisplayStateView *)D_8006C838)->hour) < 0xC;
    DrawMenuPanel((u32 *)(drawContext + 0x90), 1, 0xE4, 0x10, 0x4E, 0x20);
    y = 0x13;
    x = 0x114;
    func_80024A10(textOt, 0xE8, 0x13,
        ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->season);
    *(u16 *)func_8001A970((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->day) + 1,
        (s16 *)&gOvl11ClockHudText, 2) = 0xFFFF;
    func_80017B3C(textOt, (s32)&gOvl11ClockHudText, 0xFC, 0x13);
    *func_8001AAB8((s32)(((struct Ovl11CalendarDisplayStateView *)D_8006C838)->weekday),
        (u16 *)&gOvl11ClockHudText) = 0xFFFF;
    previousStyle = func_80017A64();
    do {
        if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->weekday) != 0) {
            style = 5;
            if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->weekday) != 6) {
                style = previousStyle;
            }
        } else {
            style = 4;
        }
        func_80017A48(style);
        func_80017B3C(textOt, (s32)&gOvl11ClockHudText, x, y);
        func_80017A48(previousStyle);
        x = 0xE8;
        y = 0x21;
        cursor = (s16 *)&gOvl11ClockHudText;
        if (beforeNoon == 0) {
            *cursor = 0xF;
        } else {
            *cursor = 0;
        }
        cursor++;
        hour = ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->hour;
        *cursor++ = 0xC;
        *cursor++ = 0xFFD;
        if (beforeNoon == 0) {
            hour -= 0xC;
        }
        do {
            cursor = func_8001A970((s32)hour, cursor, 2);
            *cursor++ = 0xFFD;
            minuteTens = (s16)((s16)(((struct Ovl11CalendarDisplayStateView *)D_8006C838)->minute) / 10);
            cursor = func_8001A970(0, func_8001A970((s32)minuteTens, cursor, 1), 1);
            display = (s16 *)&gOvl11ClockHudText;
            finalDisplay = display;
            glyphDisplay = finalDisplay;
            row = y;
            y = 0;
            *(u16 *)cursor = 0xFFFF;
            func_80017B3C(textOt, (s32)display, x, row);
            display = 0;
            finalDisplay = 0;
            minute = (s16)((struct Ovl11CalendarDisplayStateView *)D_8006C838)->minute;
            wideMinuteTens = minute / 10;
            do {
                minuteTens = (s16)wideMinuteTens;
            } while (0);
            fiveMinuteDigit = (minute - minuteTens * 10) / 5;
            gOvl11ClockHudText.leadingGlyph = 0x66;
            ((u16 *)glyphDisplay)[1] = 0xFFFF;
            func_80017D9C((s32 *)textOt, (u16 *)glyphDisplay, 0x110, row, fiveMinuteDigit);
            x = 0;
            y = 0;
        } while (repeat);
    } while (repeat);
}
