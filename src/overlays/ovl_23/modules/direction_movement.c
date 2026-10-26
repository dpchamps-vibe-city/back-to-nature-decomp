/*
 * Eight-direction movement for the six-record arena. Clearance projects only
 * Y against per-record bounds, then checks angular sectors of nearby anchors
 * and actors using the CURRENT position, not the projected position. This is
 * not a swept collision test. Direction codes: +Y, +Y+X, +X, -Y+X, -Y, -Y-X,
 * -X, +Y-X. Membership and boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl23_direction_movement.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "random.h"

s32 Ovl23CheckDirectionalClearance(Ovl23PositionedRecord *record, s32 directionWord, s32 distanceWord);
s32 Ovl23PlanarDistance(s16 x0, s16 y0, s16 x1, s16 y1);
s32 Ovl23AngleDegreesFromVector(s32 dx, s32 dy);
s32 Ovl23DirectionFromAngleDegrees(s32 degrees);

void Ovl23StepRecordInDirection(Ovl23PositionedRecord *record) {
    s16 state;
    s16 direction;
    s32 x;
    s32 diagonal;
    s32 limit;
    s32 dy;
    s32 dx;
    s32 speed;

    state = *(s16 *)((u8 *)record + 4);
    switch (state) {
    case 1:
        speed = *(s32 *)((u8 *)record + 0x10);
        break;
    case 3:
        speed = (Ovl23_D_800BF87C_bounds.unk3B2 << 12) / 10;
        break;
    default:
        return;
    }
    diagonal = (speed * 1000) / 1414;
    if (Ovl23CheckDirectionalClearance(record, *(s16 *)((u8 *)record + 6),
            Ovl23_D_800BF87C_bounds.unk39E) == 1) {
        dx = 0;
        direction = *(s16 *)((u8 *)record + 6);
        dy = 0;
        switch (direction) {
        case 0:
            dy = speed;
            break;
        case 1:
            dy = diagonal;
            dx = dy;
            break;
        case 2:
            dx = speed;
            break;
        case 3:
            dy = -diagonal;
            dx = diagonal;
            break;
        case 4:
            dy = -speed;
            break;
        case 5:
            dy = -diagonal;
            dx = dy;
            break;
        case 6:
            dx = -speed;
            break;
        case 7:
            dy = diagonal;
            dx = -dy;
            break;
        }
        x = record->xQ12 + dx;
        record->xQ12 = x;
        record->yQ12 += dy;
        limit = (Ovl23_D_800BF87C_bounds.unk39C + 0x37) << 12;
        if (x >= limit) {
            record->xQ12 = limit;
        }
        if (record->xQ12 <= -0x30000) {
            record->xQ12 = -0x30000;
        }
    }
}

s32 Ovl23ChooseRecordDirection(Ovl23PositionedRecord *record) {
    s32 x;
    s32 dx;
    s32 dy;
    s32 direction;
    Ovl23D87CView210 *arena;

    x = record->xQ12 / 4096;
    arena = (Ovl23D87CView210 *)Ovl23_D_800BF87C_draw;
    dx = x - arena->unk210[record->index].unk0 / 4096;
    dy = record->yQ12 / 4096 - ((s16 *)&gOvl23RecordAnchorY)[record->index];
    /* The discarded square-root call is present in the original. */
    SquareRoot0(dx * dx + dy * dy);
    direction = Ovl23DirectionFromRecordAnchor(record);
    if (Ovl23CheckDirectionalClearance(record, direction, (s16)(*(s16 *)((u8 *)arena + 0x39E) * 3 / 2)) != 1) {
        direction = (s16)(direction + 2 + Rand(2) * 2);
        direction = (s16)(direction % 8);
    }
    return direction;
}

s32 Ovl23CheckDirectionalClearance(Ovl23PositionedRecord *record, s32 directionWord, s32 distanceWord) {
    /* Callers pass promoted words; narrowing belongs here, not at every call. */
    s16 direction = directionWord;
    s16 distance = distanceWord;
    s16 y;
    s32 i;

    switch (direction) {
    case 3:
    case 5:
        y = record->yQ12 / 4096 - distance * 1000 / 1414;
        break;
    case 4:
        y = record->yQ12 / 4096 - distance;
        break;
    case 1:
    case 7:
        y = record->yQ12 / 4096 + distance * 1000 / 1414;
        break;
    case 0:
        y = distance + record->yQ12 / 4096;
        break;
    default:
        y = record->yQ12 / 4096;
        break;
    }
    if (y < ((s16 *)gOvl23RecordYBounds)[record->index * 2]) {
        return 0;
    }
    if (((s16 *)gOvl23RecordYBounds)[record->index * 2 + 1] < y) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (i != record->index) {
            if (distance >= Ovl23PlanarDistance(record->xQ12 / 4096,
                    record->yQ12 / 4096, Ovl23_D_800BF87C_movement.records[i].unkC / 4096,
                    ((s16 *)&gOvl23RecordAnchorY)[i])) {
                if (direction == Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(
                        record->xQ12 / 4096 - Ovl23_D_800BF87C_movement.records[i].unkC / 4096,
                        record->yQ12 / 4096 - ((s16 *)&gOvl23RecordAnchorY)[i]))) {
                    return 0;
                }
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (i != record->index) {
            if (distance >= Ovl23PlanarDistance(record->xQ12 / 4096,
                    record->yQ12 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096)) {
                if (direction == Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(
                        record->xQ12 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                        record->yQ12 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096))) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

s32 Ovl23DirectionFromRecordAnchor(Ovl23PositionedRecord *record) {
    s16 index;
    s32 adjustedY;
    s32 adjustedX;
    s32 adjustedAnchorX;
    s32 x;
    s32 anchorX;
    s32 dx;

    adjustedX = record->xQ12;
    if (adjustedX < 0) {
        adjustedX += 0xFFF;
    }
    x = adjustedX >> 12;
    index = record->index;
    adjustedAnchorX = Ovl23_D_800BF87C_movement.records[index].unkC;
    if (adjustedAnchorX < 0) {
        adjustedAnchorX += 0xFFF;
    }
    anchorX = adjustedAnchorX >> 12;
    dx = x - anchorX;
    adjustedY = record->yQ12;
    if (adjustedY < 0) {
        adjustedY += 0xFFF;
    }
    return Ovl23DirectionFromAngleDegrees(Ovl23AngleDegreesFromVector(dx,
        (adjustedY >> 12) - *(s16 *)((u8 *)&gOvl23RecordAnchorY + (index * 2))));
}

/* Seven 45-degree sectors starting at 23; the wraparound sector returns 4. */
s32 Ovl23DirectionFromAngleDegrees(s32 degrees) {
    if ((u32)(degrees - 0x17) < (u32)0x2D) {
        return 5;
    }
    if ((u32)(degrees - 0x44) < (u32)0x2D) {
        return 6;
    }
    if ((u32)(degrees - 0x71) < (u32)0x2D) {
        return 7;
    }
    if ((u32)(degrees - 0x9E) < (u32)0x2D) {
        return 0;
    }
    if ((u32)(degrees - 0xCB) < (u32)0x2D) {
        return 1;
    }
    if ((u32)(degrees - 0xF8) < (u32)0x2D) {
        return 2;
    }
    if ((u32)(degrees - 0x125) < (u32)0x2D) {
        return 3;
    }
    return 4;
}

s32 Ovl23PlanarDistance(s16 x0, s16 y0, s16 x1, s16 y1) {
    s32 distance;

    distance = SquareRoot0((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
    return ((s16)distance);
}

s32 Ovl23AngleDegreesFromVector(s32 dx, s32 dy) {
    s32 angle;

    angle = ratan2(dx, dy);
    if (angle < 0) {
        angle += 0x1000;
    }
    return (angle * 0x168) / 0x1000;
}
