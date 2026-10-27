/* Save-slot record transfer and placement. Selected save records are staged
 * out of the save-work pools before runtime records move into those pools;
 * staged records are then restored into free runtime records. The placement
 * helpers and their private tables belong to this same transfer protocol.
 * Interfaces: ovl15_save_slot_record_pool.h and ovl15_row_placement.h.
 * Membership and mechanical boundaries: notes/file-groupings.md. */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/memory.h"
#include "ovl15_row_placement.h"
#include "ovl15_save_slot_record_pool.h"
#include "random.h"

void Ovl15PlaceRowXSlot(RowXPlacement *output, s16 slot) {
    s16 row = slot / 5;
    s16 column = slot % 5;
    s32 direction;

    output->x = gOvl15RowXPlacementTable[row][0] + column * 0x190;
    output->y = gOvl15RowXPlacementTable[row][1];
    output->z = gOvl15RowXPlacementTable[row][2];
    direction = Rand(4);
    output->direction = direction;
    output->mode = 4;
}

s32 Ovl15PlaceRowZSlot(RowZPlacement *output, s16 slot) {
    s16 selected = slot;
    s16 row;
    s16 column;
    s32 mode;

    if (selected == -1) {
        row = 4;
        column = 0;
    } else if (selected == -2) {
        row = 5;
        column = 0;
    } else {
        row = selected / 5;
        column = selected % 5;
    }
    output->x = gOvl15RowZPlacementTable[row][0];
    output->y = gOvl15RowZPlacementTable[row][1];
    output->z = gOvl15RowZPlacementTable[row][2] + column * 600;
    output->direction = (row & 1) ? 1 : 3;
    mode = 5;
    output->mode = mode;
    return mode;
}

/* Internal allocation helper retains external linkage. Transfer capacity is
 * validated by the caller; these paths intentionally do not test for NULL. */
void *Ovl15FindFreePoolRecord(Ovl15RecordPools *arg0, s16 arg1, s16 *arg2);

void Ovl15StageSelectedSlotTransfers(void) {
    s16 index;
    s16 sourceIndex;
    s16 kind;
    s32 i;
    u8 *buffer;

    buffer = gOvl15StagedSlotRecords;
    index = 0;
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[1][i].unk0;
        sourceIndex = Ovl15Slots[1][i].unk2;
        if (kind != -1) {
            switch (kind) {
            case 0:
                ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)(gOvl15SaveWorkBase + sourceIndex * 0xB4), sourceIndex);
                memcpy(buffer, gOvl15SaveWorkBase + sourceIndex * 0xB4, 0xB4);
                Ovl15Slots[1][i].unk4 = (s32)buffer;
                buffer += 0xB4;
                memset(gOvl15SaveWorkBase + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                ((Ovl15SaveRecordStates *)gOvl15SaveWorkBase)->records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)(gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8), sourceIndex);
                memcpy(buffer, gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8, 0xB8);
                Ovl15Slots[1][i].unk4 = (s32)buffer;
                buffer += 0xB8;
                memset(gOvl15SaveWorkBase + 0x708 + sourceIndex * 0xB8, 0, 0xB8);
                break;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        kind = Ovl15Slots[0][i].unk0;
        sourceIndex = Ovl15Slots[0][i].unk2;
        if (kind != -1) {
            switch (kind) {
            case 0:
                buffer = Ovl15FindFreePoolRecord((Ovl15RecordPools *)gOvl15SaveWorkBase, 0, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B4[sourceIndex].state = 0;
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0xB4);
                memset((u8 *)D_8006C838 + 0x7AB4 + sourceIndex * 0xB4, 0, 0xB4);
                break;
            case 1:
            case 2:
                buffer = Ovl15FindFreePoolRecord((Ovl15RecordPools *)gOvl15SaveWorkBase, 1, &index);
                ((Ovl15TransferRuntimeStates *)D_8006C838)->pool.records_B8[sourceIndex].state = 0;
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8), index);
                memcpy(buffer, (u8 *)D_8006C838 + 0x81BC + sourceIndex * 0xB8, 0xB8);
                memset(&((Ovl15TransferRuntimeRecords *)D_8006C838)->records_B8[sourceIndex], 0, 0xB8);
                break;
            }
        }
    }
}

/* Callee scans the selected record pool and writes its index through arg2. */

void Ovl15RestoreStagedSlotRecords(void) {
    s16 index;
    s32 i;
    void *dest;

    index = 0;
    for (i = 0; i < 5; i++) {
        if (Ovl15Slots[1][i].unk0 != -1) {
            switch (Ovl15Slots[1][i].unk0) {
            case 0:
                dest = Ovl15FindFreePoolRecord((Ovl15RecordPools *)&gSmallTaggedRecordBase, 0, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].unk4, 0xB4);
                Ovl15PlaceRowXSlot((struct ovl_15_80132A8C_data *)((u8 *)&gSmallTaggedRecordBase + index * 0xB4), index);
                break;
            case 1:
            case 2:
                dest = Ovl15FindFreePoolRecord((Ovl15RecordPools *)&gSmallTaggedRecordBase, 1, &index);
                memcpy(dest, (void *)Ovl15Slots[1][i].unk4, 0xB8);
                Ovl15PlaceRowZSlot((Unk80132B44 *)((u8 *)&gTaggedRecordBase + index * 0xB8), index);
                break;
            }
        }
    }
}

void *Ovl15FindFreePoolRecord(Ovl15RecordPools *pools, s16 kind, s16 *index) {
    void *record = 0;
    s32 i;

    switch (kind) {
    case 0:
        for (i = 0; i < 10; i++) {
            if (pools->small[i].type == 0 && !(pools->small[i].flags & 0x02000000)) {
                record = &pools->small[i];
                *index = i;
                break;
            }
        }
        break;
    case 1:
        for (i = 0; i < 20; i++) {
            if (pools->large[i].type == 0 && !(pools->large[i].flags & 0x02000000)) {
                record = &pools->large[i];
                *index = i;
                break;
            }
        }
        break;
    }
    return record;
}

s16 Ovl15CountOccupiedPoolUnits(u8 *poolBase, s16 kind) {
    u8 *record;
    s16 units;
    s32 previousFixedCount;
    s32 index;
    s32 nextFixedCount;
    u32 reservedMask;

    record = poolBase;
    units = 0;
    switch (kind) {
    case 0:
        reservedMask = 0x02000000;
        nextFixedCount = 0x10000;
        for (index = 0; index < 10; index++) {
            if ((*(u16 *)record != 0) || (*(u32 *)(record + 0x34) & reservedMask)) {
                previousFixedCount = nextFixedCount;
                nextFixedCount += 0x10000;
                units = previousFixedCount >> 16;
            }
            record += 0xB4;
        }
        return units;
    case 1:
        for (index = 0; index < 20; index++) {
            if (*(u16 *)(record + 0x708) != 0) {
                units += 1;
                if (*(u16 *)(record + 0x7BA) != 0) {
                    units += 1;
                }
            }
            record += 0xB8;
        }
        break;
    default:
        break;
    }
    return units;
}
