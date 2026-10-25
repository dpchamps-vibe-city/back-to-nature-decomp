#include "common.h"
#include "game_types.h"
#include "screen_fade.h"

s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800C0F84(void);
void ovl_11_func_800CA6C4(void *arg0);
void ovl_11_func_800CF36C(s32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
void ovl_11_func_800C0FC0(s16 arg0);
void ovl_11_func_800C11C4(s32 arg0);
s32 ovl_11_func_800C1224(s16 arg0, s16 arg1);
void ovl_11_func_800F0FB0(s32 arg0);
void ovl_11_func_800C0DB4(s16 *arg0, u32 arg1);

#define STATE ((struct Ovl11ClockState *)D_8006C838)

void ovl_11_func_800C0A4C(void) {
    s16 *var_s0;
    s32 var_a2;
    s32 var_s4;
    u32 var_s1;
    struct Ovl11ClockAnchor *anchor;
    struct Ovl11ClockState *state;
    struct Ovl11ClockDate *date;
    struct Ovl11ClockPair *time;
    struct Ovl11ClockPair *other;

    STATE->flags &= 0xBDEFFFFF;
    if (D_80128B50 == 0) {
        if (IsScreenFadeComplete() == 0 || func_8001AF44(3U) == 1 ||
            (STATE->flags & 0x08000000) || ovl_11_func_800C0F84() == 1) {
            return;
        }
    } else if (D_80128B54 == 0) {
        return;
    }
    {
        anchor = (struct Ovl11ClockAnchor *)&D_8006C904;
        state = (struct Ovl11ClockState *)((u8 *)anchor - 0xCC);
        date = (struct Ovl11ClockDate *)((u8 *)anchor + 0x43EC);
        time = (struct Ovl11ClockPair *)((u8 *)anchor + 0x43F4);
        other = (struct Ovl11ClockPair *)((u8 *)anchor + 0x43F8);
        var_a2 = D_80122F78;
        state->flags |= 0x40000000;
        anchor->counter++;
        if (state->flags_10 & 0x01000000) {
            var_a2 = var_a2 / 8 + 1;
        }
        var_s4 = 0x8000;
        if (anchor->counter >= (u32)(var_a2 / 2)) {
            anchor->counter = 0;
            time->minute++;
            other->minute++;
            STATE->elapsed++;
            STATE->flags |= 0x100000;
            if (D_80128B50 == 0) {
                ovl_11_func_800CA6C4((u8 *)D_8006C838 + 0x51C8);
            }
            STATE->flags &= 0xFDFFFFFF;
            if (time->minute >= 60) {
                var_s4 = 0xC000;
                time->minute = 0;
                other->minute = 0;
                STATE->flags |= 0x02000000;
                time->hour++;
                other->hour++;
            }
            if (time->hour >= 24) {
                STATE->elapsed = 0;
                time->hour = 0;
            }
            if (time->hour == 6 && (var_s4 & 0x4000)) {
                other->hour = 0;
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
                func_8001AF70(0x96, 0);
                if (date->year >= 99) {
                    date->year = 98;
                }
            }
            ovl_11_func_800C0FC0(STATE->weekday);
            func_8001AF70(0x2B, 0);
            func_8001AF70(0x2C, 0);
            ovl_11_func_800C11C4(date->season);
            if (ovl_11_func_800C1224(STATE->season, STATE->day) != 0) {
                func_8001AF70(0x2B, 1);
            }
            var_s1 = 0;
            var_s0 = D_80122E80[0];
            do {
                ovl_11_func_800C0DB4(var_s0, 0);
                var_s1++;
                var_s0 += 7;
            } while (var_s1 < 10);
            ovl_11_func_800F0FB0(1);
        }
    }
}
