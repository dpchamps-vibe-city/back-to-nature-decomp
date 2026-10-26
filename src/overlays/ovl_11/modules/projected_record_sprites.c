/*
 * projected_record_sprites — reconstructed translation unit.
 *
 * Project record positions, clip them to the screen and select sprite frames
 * through the scene-kind callback family. The list walker, seven selectors
 * and projection/clipping helpers retain external linkage; sharing this file
 * is not evidence that the original functions were static.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_projected_record_sprites.h"
#include "point_projection.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

s32 Ovl11IsRecordSpriteOnScreen(s32 x, s32 y) {
    s32 visible;

    visible = 0;
    if ((u32) (x + 0x31) < 0x1A3U) {
        visible = (u32) (y + 0x31) < 0x153U;
    }
    return visible;
}

s32 Ovl11ProjectRecordSprite(u16 *position, s32 *screen) {
    s32 depth;

    gOvl11RecordSpriteScreen[1] = 0x5DC;
    gOvl11RecordSpriteScreen[0] = 0x5DC;
    gOvl11RecordSpritePosition[0] = position[0];
    gOvl11RecordSpritePosition[1] = position[1];
    gOvl11RecordSpritePosition[2] = position[2];
    depth = ProjectPointToScreen(gOvl11RecordSpriteScreen, gOvl11RecordSpritePosition);
    if (depth > 0) {
        if (Ovl11IsRecordSpriteOnScreen(gOvl11RecordSpriteScreen[0], gOvl11RecordSpriteScreen[1]) != 0) {
            screen[0] = gOvl11RecordSpriteScreen[0];
            screen[1] = gOvl11RecordSpriteScreen[1];
            screen[2] = depth;
            return 1;
        }
    }
    return 0;
}

void Ovl11DrawProjectedRecordList(s32 count, Ovl11ProjectedRecordView *records, s32 (*selectSprite)(s32, s32 *, s32 *));
s32 Ovl11SelectKind5Sprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectKind8Sprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectSeasonSprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectKind7Sprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectKind6Sprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectKind4Sprite(s32 arg0, s32 *arg1, s32 *arg2);
s32 Ovl11SelectKind9Sprite(s32 arg0, s32 *arg1, s32 *arg2);

void Ovl11DrawSceneRecordSprites(void) {
    Ovl11DispatchRecord record;
    s16 selector;
    s16 mode;
    s32 (*callback)(s32, s32 *, s32 *);
    s32 index;

    mode = ((struct Ovl11DispatchWork *) &D_8007AFF0)->header[5];
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
        callback = Ovl11SelectSeasonSprite;
        break;
    case 4:
        callback = Ovl11SelectKind4Sprite;
        break;
    case 5:
        callback = Ovl11SelectKind5Sprite;
        break;
    case 6:
        callback = Ovl11SelectKind6Sprite;
        break;
    case 7:
        callback = Ovl11SelectKind7Sprite;
        break;
    case 8:
        callback = Ovl11SelectKind8Sprite;
        break;
    case 9:
        callback = Ovl11SelectKind9Sprite;
        break;
    default:
        return;
    }

    selector = ((struct Ovl11DispatchWork *) &D_8007AFF0)->selector - 1;
    switch (selector) {
    case 0:
        index = 0;
        break;
    case 1:
        index = 1;
        break;
    case 3:
        index = 3;
        break;
    case 4:
        index = 2;
        break;
    case 0x2F:
        index = 4;
        break;
    case 7:
        index = 5;
        break;
    case 0x13:
        index = 7;
        break;
    case 0x19:
        index = 9;
        break;
    case 0x1C:
        index = 8;
        break;
    case 0x1D:
        index = 0xA;
        break;
    case 0x21:
        index = 6;
        break;
    case 0x33:
        index = 0xB;
        break;
    case 0xB:
        index = 0xC;
        break;
    case 0xC:
        index = 0xD;
        break;
    case 0xD:
        index = 0xE;
        break;
    case 0xE:
        index = 0xF;
        break;
    default:
        index = -1;
        break;
    }

    if (index >= 0) {
        Ovl11DrawProjectedRecordList(
            ((struct Ovl11DispatchState *) D_8006C838)->counts[index],
            (Ovl11ProjectedRecordView *) ((struct Ovl11DispatchState *) D_8006C838)->lists[index],
            callback);
    }
    Ovl11DrawProjectedRecordList(1,
        (Ovl11ProjectedRecordView *) (((struct Ovl11DispatchState *) D_8006C838)->fixed + 1),
        callback);
    record = *((struct Ovl11DispatchState *) D_8006C838)->fixed;
    if (record.flags & 0x100) {
        record.y += gOvl11RecordSpriteYOffsets[gOvl11RecordSpriteYOffsetIndex];
    }
    Ovl11DrawProjectedRecordList(1, (Ovl11ProjectedRecordView *) &record,
        callback);
    Ovl11DrawProjectedRecordList(0xA,
        (Ovl11ProjectedRecordView *) ((struct Ovl11DispatchState *) D_8006C838)->list10,
        callback);
}

s32 ovl_11_func_800F581C(void);
void Ovl11DrawProjectedRecordList(s32 count, Ovl11ProjectedRecordView *records, s32 (*selectSprite)(s32, s32 *, s32 *)) {
    u16 position[3];
    s32 screen[3];
    s32 frame;
    s32 sourceIndex;
    s32 requiredFlags;
    s32 remaining;
    char *workBase;
    s32 (*callback)(s32, s32 *, s32 *);

    requiredFlags = (ovl_11_func_800F581C() | 1) & 0xFFFF;
    if ((selectSprite != 0) && (count > 0)) {
        callback = selectSprite;
        workBase = (char *) D_8009AFF0;
        remaining = count;
        do {
            if (((records->flags & requiredFlags) == requiredFlags) && (records->selectionKey == (*(s16 *) (workBase + 0x5476)))) {
                position[0] = records->xLow + 0x96;
                position[1] = records->yLow - 0x64;
                position[2] = records->zLow - 0x96;
                if (Ovl11ProjectRecordSprite(position, screen) != 0) {
                    if (callback(records->spriteKind, &frame, &sourceIndex) != 0) {
                        /* The selectors write a word; rendering consumes its low byte. */
                        DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4), (s32) (&gSpriteSourceBankBase + sourceIndex), (s32) (u8) frame, 0, (s16) screen[0], (s16) screen[1]);
                    }
                }
            }
            remaining -= 1;
            records += 1;
        } while (remaining != 0);
    }
}

/* Selector convention: arg0 is a sprite-kind code, arg1 receives a frame
 * word, arg2 a source-bank index. Return zero for an unsupported code.
 * Scene-kind names below describe the dispatch cases, not gameplay roles. */
s32 Ovl11SelectKind5Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_a3;

    var_a3 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x3A:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 0;
        break;
    case 0x3B:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 1;
        break;
    case 0x3C:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 2;
        break;
    case 0x3D:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 3;
        break;
    case 0x3E:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 4;
        break;
    case 0x40:
        if (gCalendarSeason == 2) {
            *arg1 = (s32) gCalendarSeason;
        }
        *arg2 = 5;
        break;
    case 0x51:
        *arg2 = 6;
        break;
    case 0x52:
        *arg2 = 7;
        break;
    case 0x53:
        *arg2 = 8;
        break;
    case 0x54:
        *arg2 = 0x13;
        break;
    case 0x55:
        *arg2 = 9;
        break;
    case 0x56:
        *arg2 = 0xA;
        break;
    case 0x58:
        *arg2 = 0xB;
        break;
    case 0x59:
        *arg2 = 0xC;
        break;
    case 0x5A:
        *arg2 = 0xD;
        break;
    case 0x5B:
        *arg2 = 0xE;
        break;
    case 0x5C:
        *arg2 = 0xF;
        break;
    case 0x5D:
        *arg2 = 0x10;
        break;
    case 0x5E:
        *arg2 = 0x11;
        break;
    case 0x5F:
        *arg2 = 0x12;
        break;
    case 0x65:
        *arg2 = 0x15;
        break;
    case 0x64:
        *arg2 = 0x20;
        break;
    default:
        var_a3 = 0;
        break;
    }
    return var_a3;
}

s32 Ovl11SelectKind8Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 result;

    result = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x47:
        *arg2 = 0x17;
        break;
    case 0x51:
        break;
    case 0x52:
        *arg2 = 1;
        break;
    case 0x53:
        *arg2 = 2;
        break;
    case 0x59:
    case 0x139:
        *arg2 = 0x1D;
        break;
    case 0x84:
        *arg2 = 7;
        break;
    case 0x6D:
        *arg2 = 8;
        break;
    case 0x6E:
        *arg2 = 9;
        break;
    case 0x177:
        *arg2 = 0xA;
        break;
    case 0xA5:
        *arg2 = 0xC;
        break;
    case 0xA6:
        *arg2 = 0xD;
        break;
    case 0xA7:
        *arg2 = 0xE;
        break;
    case 0x128:
        *arg2 = 0xF;
        break;
    case 0x129:
        *arg2 = 0x10;
        break;
    case 0x12A:
        *arg2 = 0x11;
        break;
    case 0x54:
        *arg2 = 0xB;
        break;
    case 0x64:
        *arg2 = 0x18;
        break;
    case 0x9A:
        *arg2 = 0x19;
        break;
    case 0x99:
        *arg2 = 0x1A;
        break;
    case 0x21:
        *arg2 = 0x1B;
        break;
    case 0x6F:
        *arg2 = 0x1C;
        break;
    case 0x65:
        *arg2 = 0x1E;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

s32 Ovl11SelectSeasonSprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_a3;
    s32 state;
    Recon_ovl_11_func_800F6118_D80126E18Entry *temp_v1;

    var_a3 = 1;
    state = gCalendarSeason;
    *arg1 = 0;
    *arg2 = 0;
    if ((u32) state >= 4U) {
        return 0;
    }
    temp_v1 = &D_80126E18[state];
    switch (arg0) {
    case 0x83:
        *arg2 = (s32) temp_v1->unk0;
        break;
    case 0x57:
        *arg2 = 0x12;
        break;
    case 0x64:
        *arg2 = (s32) temp_v1->unk1;
        break;
    case 0x65:
        *arg2 = (s32) temp_v1->unk2;
        break;
    case 0x122:
    case 0x123:
    case 0x124:
        *arg2 = (s32) temp_v1->unk3;
        break;
    case 0xE8:
        *arg2 = (s32) temp_v1->unk4;
        break;
    default:
        var_a3 = 0;
        break;
    }
    return var_a3;
}

s32 Ovl11SelectKind7Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_v1;

    var_v1 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x68:
        *arg2 = 4;
        break;
    case 0x69:
        *arg2 = 7;
        break;
    case 0x64:
        *arg2 = 2;
        break;
    case 0x65:
        *arg2 = 3;
        break;
    case 0xE8:
        *arg2 = 6;
        break;
    case 0x122:
    case 0x123:
    case 0x124:
        *arg2 = 5;
        break;
    default:
        var_v1 = 0;
        break;
    }
    return var_v1;
}

s32 Ovl11SelectKind6Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_a0;
    s32 var_a3;
    s32 var_t0;

    var_t0 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x64:
        break;
    case 0x65:
        *arg2 = 1;
        break;
    case 0x188:
        *arg2 = 0x14;
        break;
    case 0x71:
        var_a0 = 0;
        var_a3 = 0x13;
        switch ((u16)D_80070D0E) {
        case 0x51:
            var_a0 = 0;
            break;
        case 0x55:
            var_a0 = 1;
            break;
        case 0x52:
            var_a0 = 2;
            break;
        case 0x53:
            var_a0 = 4;
            break;
        case 0x54:
            var_a0 = 5;
            break;
        default:
            var_a3 = 3;
            break;
        }
        *arg1 = var_a0;
        *arg2 = var_a3;
        break;
    case 0x72:
        *arg2 = 4;
        break;
    case 0x73:
        *arg2 = 5;
        break;
    case 0x74:
        *arg2 = 6;
        break;
    case 0x75:
        *arg2 = 7;
        break;
    case 0x76:
        *arg2 = 8;
        break;
    case 0x77:
        *arg2 = 9;
        break;
    case 0x78:
        *arg2 = 0xA;
        break;
    case 0x79:
        *arg2 = 0xB;
        break;
    case 0x7D:
        *arg2 = 0xC;
        break;
    case 0x7E:
        *arg2 = 0xD;
        break;
    case 0x7F:
        *arg2 = 0xE;
        break;
    case 0x80:
        *arg2 = 0xF;
        break;
    case 0x81:
        *arg2 = 0x10;
        break;
    case 0x82:
        *arg2 = 0x11;
        break;
    default:
        var_t0 = 0;
        break;
    }
    return var_t0;
}

s32 Ovl11SelectKind4Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_v1;

    var_v1 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x64:
        *arg2 = 0x19;
        break;
    case 0x65:
        *arg2 = 0x1A;
        break;
    case 0xE8:
        *arg2 = 0x1E;
        break;
    case 0x122:
    case 0x123:
    case 0x124:
        *arg2 = 0x1D;
        break;
    default:
        var_v1 = 0;
        break;
    }
    return var_v1;
}

s32 Ovl11SelectKind9Sprite(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_v1;

    var_v1 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x64:
        *arg2 = 0x11;
        break;
    case 0x65:
        *arg2 = 0x13;
        break;
    default:
        var_v1 = 0;
        break;
    }
    return var_v1;
}
