/*
 * object_id_state — reconstructed translation unit.
 *
 * Object-ID state selection, sprite-source refresh and movement-step lookup,
 * including the dispatcher's four weighted random choices and clock/value
 * choice. Original ovl_11:800D3468-800D3D60. The former random_state_choice
 * module is part of this complete service, not a separate source boundary.
 * Public contracts: ovl11_object_id_state.h / ovl11_random_state_choice.h.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_id_state.h"
#include "psyq/libapi.h"
#include "random.h"
#include "ovl11_random_state_choice.h"
#include "ovl11_object_state_dispatch.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "psyq/abs.h"
#include "ovl11_object_event_script.h"

s32 Ovl11ChooseStateByClockAndValue(Ovl11ObjectIdCondition *arg0);

s32 Ovl11EnterObjectStateForId(u16 *arg0) {
    s32 var_s0;

    var_s0 = 0;
    switch (arg0[0]) {
    case 0x154:
        var_s0 = 2;
        break;
    case 0x156:
        var_s0 = 3;
        break;
    case 0x157:
        var_s0 = 4;
        break;
    case 0x15B:
        var_s0 = Ovl11ChooseStateByClockAndValue((Ovl11ObjectIdCondition *)arg0);
        break;
    case 0x108:
        if (Rand(2) != 0) {
            var_s0 = 0x11;
        } else {
            var_s0 = 1;
        }
        break;
    case 0x10B:
    case 0x10C:
        var_s0 = Ovl11RollState8(arg0);
        break;
    case 0x10D:
        var_s0 = Ovl11RollState10(arg0);
        break;
    case 0x10E:
        var_s0 = Ovl11RollState11(arg0);
        break;
    case 0x10F:
        var_s0 = Ovl11RollState13(arg0);
        break;
    case 0x112:
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x155:
        var_s0 = 0;
        break;
    case 0x110:
    case 0x159:
    case 0x162:
    case 0x165:
    case 0x17A:
    case 0x17B:
        var_s0 = 1;
        break;
    default:
        SystemError(0x45, 0x64);
        break;
    }
    Ovl11EnterObjectState(arg0, var_s0);
    return 0;
}

void Ovl11RefreshObjectSpriteForId(u16 *arg0) {
    SpriteSourceData sp10; /* script-selected source */
    s32 *var_s1;           /* selected sprite bank */
    s32 var_s0;            /* byte offset within that bank */
    u16 temp_v1;           /* ID before source availability can clear it */

    var_s1 = NULL;
    temp_v1 = *arg0;
    var_s0 = 0;
    switch (temp_v1) {
    case 0x154:
        var_s1 = &gFixedIdSpriteBankBase;
        var_s0 = gOvl11FixedIdSpriteOffsets[5];
        break;
    case 0x156:
        var_s1 = &gFixedIdSpriteBankBase;
        var_s0 = gOvl11FixedIdSpriteOffsets[4];
        break;
    case 0x157:
        var_s1 = &gFixedIdSpriteBankBase;
        var_s0 = gOvl11FixedIdSpriteOffsets[3];
        break;
    case 0x15B:
        var_s1 = &gAreaSpriteWorkBase;
        var_s0 = 0;
        break;
    case 0x10B:
    case 0x10C:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        switch (gCalendarSeason) {
        case 0:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason0[3];
            break;
        case 1:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason1[2];
            break;
        case 2:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason2[3];
            break;
        case 3:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason3[3];
            break;
        }
        break;
    case 0x10D:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        switch (gCalendarSeason) {
        case 0:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason0[1];
            break;
        case 1:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason1[1];
            break;
        case 2:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason2[2];
            break;
        case 3:
            SystemError(0x45, 1);
            break;
        }
        break;
    case 0x10E:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        switch (gCalendarSeason) {
        case 0:
            SystemError(0x45, 2);
            break;
        case 1:
            SystemError(0x45, 3);
            break;
        case 2:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason2[1];
            break;
        case 3:
            var_s0 = gOvl11ObjectSpriteOffsetsSeason3[1];
            break;
        }
        break;
    case 0x10F:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 3) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason3[2];
        } else {
            SystemError(0x45, 4);
        }
        break;
    case 0x110:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 0) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason0[2];
        } else {
            SystemError(0x45, 5);
        }
        break;
    case 0x114:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 1) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason1[3];
        } else {
            SystemError(0x45, 6);
        }
        break;
    case 0x115:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 1) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason1[4];
        } else {
            SystemError(0x45, 7);
        }
        break;
    case 0x116:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 1) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason1[5];
        } else {
            SystemError(0x45, 8);
        }
        break;
    case 0x117:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 2) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason2[5];
        } else {
            SystemError(0x45, 9);
        }
        break;
    case 0x118:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 2) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason2[4];
        } else {
            SystemError(0x45, 8);
        }
        break;
    case 0x11A:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 0) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason0[5];
        } else {
            SystemError(0x45, 0xB);
        }
        break;
    case 0x119:
        var_s1 = &gSeasonalObjectSpriteBankBase;
        if (gCalendarSeason == 0) {
            var_s0 = gOvl11ObjectSpriteOffsetsSeason0[4];
        } else {
            SystemError(0x45, 0xC);
        }
        break;
    case 0x108:
    case 0x112:
    case 0x113:
    case 0x153:
    case 0x155:
    case 0x159:
    case 0x162:
    case 0x165:
    case 0x17A:
    case 0x17B:
        break;
    default:
        SystemError(0x45, 0);
        break;
    }
    switch (*arg0) {
    case 0x162:
        if (D_80070CF8_word == 6 || Ovl11SelectScriptSpriteSource(0x28, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x165:
        if (D_80070CF8_word == 6 || Ovl11SelectScriptSpriteSource(0x29, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x108:
        if (D_80070CF8_word == 6 || Ovl11SelectScriptSpriteSource(0x2B, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x17A:
        if (D_80070CF8_word == 6 || Ovl11SelectScriptSpriteSource(0x2A, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x17B:
        if (D_80070CF8_word == 6 || Ovl11SelectScriptSpriteSource(0x2C, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    default:
        if (*(s32 *) ((u8 *) arg0 + 0x8C) == 0) {
            InitSpriteSourceData((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) ((u8 *) var_s1 + var_s0));
        }
        break;
    }
}

s32 Ovl11GetObjectMoveStep(u16 *object) {
    u16 id;

    id = *object;
    switch (id) {
    case 0x15B:
        return 0x18;
    case 0x108:
    case 0x10B:
    case 0x10C:
    case 0x10D:
    case 0x10E:
    case 0x10F:
    case 0x110:
    case 0x112:
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x155:
    case 0x159:
    case 0x17A:
    case 0x17B:
        return 0xC;
    case 0x162:
    case 0x165:
        return 6;
    default:
        return 0xC;
    }
}

/*
 * Flag-weighted state choices for the four related object-code branches.
 * Each draws once, mapping only the first one/two rolls to special states.
 * Keep the explicit result assignments: these are the original decision
 * trees, not a table-driven choice or a second draw on fallback.
 * Contract: ovl11_random_state_choice.h. Original ovl_11:800D3C04-800D3D2C.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
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

s32 Ovl11ChooseStateByClockAndValue(Ovl11ObjectIdCondition *object) {
    s32 state;

    state = 0x14;
    if ((gClockHour < 7) || (object->valueAC <= 0)) {
        state = 0x13;
    }
    return state;
}
