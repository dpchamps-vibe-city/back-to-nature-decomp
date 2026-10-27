/*
 * Table-driven record placement and the associated kind/index selectors.
 *
 * Two slot-to-position maps use five columns, with 600-unit Z or 400-unit X
 * spacing. A third map installs a record's position, facing and kind 0x28.
 * The adjacent selectors derive message-window ids and movement periods from
 * that record protocol. The item/header reset is shared by the placement
 * controller and the separately installed item sprite source.
 *
 * ovl11_record_position_tables.h describes the unchecked indices and effects.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_record_position_tables.h"

void Ovl11GetSlotPositionZ600(s32 *position, s16 slot) {
    s16 index = slot;
    s16 row;
    s16 column;

    if (index == 20) {
        row = 4;
        column = 0;
    } else if (index == 21) {
        row = 5;
        column = 0;
    } else {
        row = index / 5;
        column = index % 5;
    }
    position[0] = gOvl11SlotPositionsZ600[row][0];
    position[1] = gOvl11SlotPositionsZ600[row][1];
    position[2] = gOvl11SlotPositionsZ600[row][2] + column * 600;
}

void Ovl11GetSlotPositionX400(s32 *position, s16 slot) {
    s16 row = slot / 5;
    s16 column = slot % 5;

    position[0] = gOvl11SlotPositionsX400[row][0] + column * 400;
    position[1] = gOvl11SlotPositionsX400[row][1];
    position[2] = gOvl11SlotPositionsX400[row][2];
}

void Ovl11ClearRecordItemSprite(u8 *record) {
    *(u16 *)(record + 0xB6) = 0;
    /* +DC is the header in the embedded SpriteSourceData at +C8. */
    *(u32 *)(record + 0xDC) = 0;
}

void Ovl11PlaceRecordFromTable(Ovl11TablePositionObject *record) {
    s32 coordinate;
    u16 facing;

    coordinate = gOvl11RecordPlacementTable[record->tableIndex].x;
    record->x = coordinate;
    coordinate = gOvl11RecordPlacementTable[record->tableIndex].y;
    record->y = coordinate;
    coordinate = gOvl11RecordPlacementTable[record->tableIndex].z;
    record->z = coordinate;
    facing = *(u16 *)((char *)&gOvl11RecordPlacementTable[record->tableIndex] + 0xC);
    record->placementKind = 0x28;
    record->facing = facing;
}

void Ovl11SelectRecordMessage(Ovl11DE68Object *record, s32 *archive, s32 *entry) {
    s32 value;

    value = 0;
    if (record->unk30 == 0x28) {
        if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->season == 3 &&
            ((struct Ovl11CalendarDisplayStateView *) D_8006C838)->day >= 0x1C) {
            *archive = 0;
            *entry = 2;
        } else if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->season != 0) {
            *archive = 0;
            *entry = 0;
        } else {
            *archive = 0;
            *entry = 2;
        }
    } else if (record->unkB8 & 0x40) {
        switch (record->unk26) {
        case 2:
            *archive = 0;
            *entry = 0xA;
            record->unk2C = 0x12C;
            record->unk24 = 0;
            record->unk34 &= ~0x800;
            break;
        case 3:
            *archive = 0;
            *entry = 0xC;
            break;
        case 4:
        case 7:
            *archive = 0;
            *entry = 8;
            break;
        default:
            *archive = 0;
            *entry = 8;
            break;
        }
    } else {
        /* Lowest set selector bit wins; absent bits leave value at zero. */
        if (record->unkB8 & 1) {
            value = record->unkAE;
        } else if (record->unkB8 & 2) {
            value = record->unkB0;
        } else if (record->unkB8 & 4) {
            value = record->unkB2;
        }
        if (value < 0xC8) {
            *archive = 0;
            *entry = 4;
        } else {
            *archive = 0;
            *entry = 6;
        }
    }
    *entry += gOvl11RecordMessageBases[record->unkAC];
    if (record->unkB8 & 8) {
        *entry = *entry + 1;
    }
}

s32 Ovl11GetRecordMovePeriod(s32 recordAddress) {
    /* The same +AE/B0/B2 cells are deliberately read unsigned here, unlike
     * the signed message-threshold view above. Retain both interpretations. */
    switch (*(s16 *)(recordAddress + 0x26)) {
    case 8:
        recordAddress = *(u16 *)(recordAddress + 0xAE);
        break;
    case 9:
        recordAddress = *(u16 *)(recordAddress + 0xB0);
        break;
    case 10:
        recordAddress = *(u16 *)(recordAddress + 0xB2);
        break;
    default:
        return 24;
    }

    return ((Ovl11Value99DAView *)D_8006C838)->values[recordAddress / 52] * 2;
}
