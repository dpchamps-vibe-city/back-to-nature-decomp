/*
 * timer_hud — reconstructed translation unit.
 *
 * Private elapsed-tick counter and MM:SS:centisecond HUD using a nominal 30
 * ticks per second, with separate reset, advance and draw modes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl17_timer_hud.h"

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Ovl17UpdateTimerHud(s16 mode) {
    s32 textOrderingTable;

    textOrderingTable = gActiveRenderContext->field_D8 + 8;
    if (mode == -1) {
        gOvl17TimerTicks = 0;
    }
    if (mode == 0 || mode == 2) {
        gOvl17TimerTicks++;
    }
    if ((u16)mode < 2) {
        /* Unsigned divisions keep the nominal 30-tick time conversion and
         * minute-within-hour wrap, including the original u32 arithmetic. */
        func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0xC), 1, 0xA, 0x14, 0x6B, 0x11);
        func_8001AAF4(gOvl17TimerTicks / 1800 - (gOvl17TimerTicks / 108000) * 60, 2, textOrderingTable, 0xC, 0x16);
        func_80017B3C(textOrderingTable, (s32)gOvl17TimerSeparatorText, 0x26, 0x16);
        func_8001AAF4(gOvl17TimerTicks / 300 - (gOvl17TimerTicks / 1800) * 6, 1, textOrderingTable, 0x33, 0x16);
        func_8001AAF4(gOvl17TimerTicks / 30 - (gOvl17TimerTicks / 300) * 10, 1, textOrderingTable, 0x40, 0x16);
        func_80017B3C(textOrderingTable, (s32)gOvl17TimerSeparatorText, 0x4D, 0x16);
        func_8001AAF4(gOvl17TimerTicks / 3 - (gOvl17TimerTicks / 30) * 10, 1, textOrderingTable, 0x5A, 0x16);
        func_8001AAF4((gOvl17TimerTicks * 100) / 30 - (gOvl17TimerTicks / 3) * 10, 1, textOrderingTable, 0x67, 0x16);
    }
}
