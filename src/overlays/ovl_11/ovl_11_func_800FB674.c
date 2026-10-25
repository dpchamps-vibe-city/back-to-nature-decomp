#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80024A10 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u16 *func_8001AAB8 (s32 arg0, u16 *arg1);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
void func_80017D9C (s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4);
void func_800136D4 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);

void ovl_11_func_800FB674(void) {
    s16 x, y;
    s32 repeat;
    s16 *temp_v0;
    s16 *display, *final_display, *glyph_display;
    s16 row, final_row, quotient, final_quotient;
    u16 terminator;
    s32 var_a0_2;
    s32 digit, minute;
    s32 wideQuotient;
    s32 call_object, final_object;
    s32 temp_s5;
    s32 temp_s7;
    s32 temp_t0;
    u32 temp_s1;
    u32 var_a0;

    repeat = 0;
        temp_t0 = D_8005E3C0->field_D8;
        temp_s5 = temp_t0 + 0x8C;
        temp_s7 = (((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C0) < 0xC;
        func_80022580((u32 *)(temp_t0 + 0x90), 1, 0xE4, 0x10, 0x4E, 0x20);
        y = 0x13;
        x = 0x114;
        func_80024A10(temp_s5, 0xE8, 0x13, ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BA);
        *(u16 *)func_8001A970((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BC) + 1, (s16 *)&D_8012A008, 2) = 0xFFFF;
        func_80017B3C(temp_s5, (s32) &D_8012A008, 0xFC, 0x13);
        *func_8001AAB8((s32) (((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE), (u16 *) &D_8012A008) = 0xFFFF;
        temp_s1 = func_80017A64();
    do {
        if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE) != 0) {
            var_a0 = 5;
            if ((((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44BE) != 6) {
                var_a0 = temp_s1;
            }
        } else {
            var_a0 = 4;
        }
        func_80017A48(var_a0);
        func_80017B3C(temp_s5, (s32) &D_8012A008, x, y);
        func_80017A48(temp_s1);
        x = 0xE8;
        y = 0x21;
        temp_v0 = (s16 *)&D_8012A008;
        if (temp_s7 == 0) {
            *temp_v0 = 0xF;
        } else {
            *temp_v0 = 0;
        }
        temp_v0++;
        var_a0_2 = ((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C0;
        *temp_v0++ = 0xC;
        *temp_v0++ = 0xFFD;
        if (temp_s7 == 0) {
            var_a0_2 -= 0xC;
        }
        do {
        temp_v0 = func_8001A970((s32) var_a0_2, temp_v0, 2);
        *temp_v0++ = 0xFFD;
        quotient = (s16) ((s16)(((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C2) / 10);
        temp_v0 = func_8001A970(0, func_8001A970((s32)quotient, temp_v0, 1), 1);
        display = (s16 *)&D_8012A008;
        final_display = display;
        glyph_display = final_display;
        row = y;
        y = 0;
        *(u16 *)temp_v0 = 0xFFFF;
        func_80017B3C(temp_s5, (s32)display, x, row);
        display = 0;
        final_display = 0;
        minute = (s16)((struct Ovl11CalendarDisplayStateView *)D_8006C838)->field_44C2;
        wideQuotient = minute / 10;
        do {
            quotient = (s16)wideQuotient;
        } while (0);
        digit = (minute - quotient * 10) / 5;
        D_8012A008.field_0 = 0x66;
        ((u16 *)glyph_display)[1] = 0xFFFF;
        func_80017D9C((s32 *) temp_s5, (u16 *)glyph_display, 0x110, row, digit);
        x = 0;
        y = 0;
        } while (repeat);
    } while (repeat);
}
