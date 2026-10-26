/*
 * Flag-weighted state choices for the four related object-code branches.
 * Each draws once, mapping only the first one/two rolls to special states.
 * Keep the explicit result assignments: these are the original decision
 * trees, not a table-driven choice or a second draw on fallback.
 * Contract: ovl11_random_state_choice.h. Original ovl_11:800D3C04-800D3D2C.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "random.h"
#include "ovl11_random_state_choice.h"

s32 Ovl11RollState8(void *object) {
    s32 range = 3;
    s32 roll;
    s32 state;

    if (*(u32 *)((char *)object + 0x34) & 0x2000) {
        range = 8;
    }
    roll = Rand(range);
    state = 8;
    if (roll != 0) {
        state = 1;
        if (roll == 1) {
            state = 9;
        }
    }
    return state;
}

s32 Ovl11RollState11(void *object) {
    s32 range = 3;
    s32 roll;
    s32 state;

    if (*(u32 *)((char *)object + 0x34) & 0x2000) {
        range = 8;
    }
    roll = Rand(range);
    state = 11;
    if (roll != 0) {
        state = 1;
        if (roll == 1) {
            state = 12;
        }
    }
    return state;
}

s32 Ovl11RollState10(void *object) {
    s32 range = 2;
    s32 state;

    if (*(u32 *)((char *)object + 0x34) & 0x2000) {
        range = 7;
    }
    if (Rand(range) == 0) {
        state = 10;
    } else {
        state = 1;
    }
    return state;
}

s32 Ovl11RollState13(void *object) {
    s32 range = 2;
    s32 state;

    if (*(u32 *)((char *)object + 0x34) & 0x2000) {
        range = 7;
    }
    if (Rand(range) == 0) {
        state = 13;
    } else {
        state = 1;
    }
    return state;
}
