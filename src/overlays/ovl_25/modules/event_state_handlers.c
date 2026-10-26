/*
 * event_state_handlers — reconstructed translation unit.
 *
 * Six ordered callbacks for the private event-phase table: seed records,
 * request messages, apply lower/upper camera bounds, and begin the two screen
 * fades. The outer dispatcher owns completion and reset.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl25_event_state_handlers.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "message_window.h"

void ovl_25_func_800B8478 (void);

void Ovl25BeginEventPhases(void) {
    ovl_25_func_800B8478();
    gOvl25EventPhase = 1;
}

void Ovl25WaitEventPrompt31(void) {
    s32 var_v1;

    RequestMessageWindow(4, 0x31);
    var_v1 = GetMessageWindowStatus();
    if (var_v1 == 2) {
        gOvl25EventPhase = var_v1;
    }
}

void Ovl25MoveEventTargetForward(void) {
    char *base;
    char *p;
    char *ref;
    s16 value;

    base = (char *)&D_8007AFF0;
    p = base + 0x20000;
    ref = (char *)&D_800BFE44_view;
    value = *(s16 *)(p + 0x53B4);
    if (value >= *(s16 *)(ref + 0x4D2) - 0x140) {
        *(u16 *)(p + 0x53B4) = *(u16 *)(ref + 0x4D2) - 0x140;
        RequestMessageWindow(4, 0x32);
        if (GetMessageWindowStatus() == 2) {
            *(s16 *)(ref + 2) = 3;
            D_800BCD21 = 1;
            D_800BCD48[0] = 0x10CC;
            D_800BCD48[1] = 0;
            D_800BCD48[2] = 0x708;
        }
    } else {
        *(u16 *)(p + 0x53B4) = *(u16 *)(p + 0x53B4) + 0x21;
    }
}

void Ovl25MoveEventTargetBack(void) {
    char *base;
    char *p;
    char *ref;
    s16 value;

    base = (char *)&D_8007AFF0;
    p = base + 0x20000;
    ref = (char *)&D_800BFE44_view;
    value = *(s16 *)(p + 0x53B4);
    if (value <= *(s16 *)(ref + 0x20)) {
        *(u16 *)(p + 0x53B4) = *(u16 *)(ref + 0x20);
        RequestMessageWindow(4, 0x33);
        if (GetMessageWindowStatus() == 2) {
            if (GetMessageWindowChoice() == 1) {
                *(s16 *)(ref + 2) = 4;
            } else {
                *(s16 *)(ref + 2) = 0;
            }
        }
    } else {
        *(u16 *)(p + 0x53B4) = *(u16 *)(p + 0x53B4) - 0x21;
    }
}

void Ovl25BeginEventFadeIncrease(void) {
    RequestMessageWindow(4, 0x34);
    if (GetMessageWindowStatus() == 2) {
        BeginScreenFadeIncrease(0xA, 0, 2);
        gOvl25EventPhase = 5;
    }
}

void ovl_25_func_800B83A0(void);

void Ovl25WaitAndBeginEventFadeDecrease(void) {
    char *far_base;

    if (IsScreenFadeComplete() == 1) {
        ovl_25_func_800B83A0();
        BeginScreenFadeDecrease(0xA, 0, 2);
        far_base = (char *)&D_8007AFF0;
        *(s16 *)(far_base + 0x253B4) = -0x140;
        gOvl25EventPhase = 6;
    }
}
