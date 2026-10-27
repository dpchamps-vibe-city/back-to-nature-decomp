/*
 * Paired record selection slots, ovl_11:800D075C..800D0DCC.
 * Slots 0/1 refer to the B8-stride tagged pool; slots 2/3 to the B4 pool.
 * The same two expansion flags govern the second slot and second pool half.
 * Placement, selection restore, availability tests and pair writes belong to
 * this protocol. Pool scans only FIND a record: callers must reserve it.
 */
#include "ovl11_record_pair_slots.h"
#include "ovl11_row_placement.h"
#include "ovl11_tagged_record_creation.h"
#include "ovl11_code160_record_creation.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_item_record_allocation.h"

void Ovl11ApplyRecordRowPlacement(s32 record, s16 rowSlot, s32 poolKind) {
    if (poolKind == 1) {
        Ovl11PlaceRowZSlot(record, rowSlot);
    } else {
        Ovl11PlaceRowXSlot(record, rowSlot);
    }
}

void Ovl11ResyncRecordRowPlacements(s32 poolKind) {
    s32 i;
    u8 *record;
    s16 rowSlot;
    s16 selectedMode;
    s32 firstSlot;
    s32 recordAddress;

    if (poolKind == 1) {
        i = 0;
        selectedMode = 5;
        record = (u8 *)&gTaggedRecordBase;
        do {
            if (*(u16 *)(record + 0) != 0 && *(s16 *)(record + 0x30) == selectedMode) {
                Ovl11ApplyRecordRowPlacement((s32)record, (s16)i, 1);
            }
            i++;
            record += 0xB8;
        } while (i < 0x14);
    } else {
        i = 0;
        selectedMode = 4;
        record = (u8 *)&gSmallTaggedRecordBase;
        do {
            if (*(u16 *)(record + 0) != 0 && *(s16 *)(record + 0x30) == selectedMode) {
                Ovl11ApplyRecordRowPlacement((s32)record, (s16)i, 0);
            }
            i++;
            record += 0xB4;
        } while (i < 0xA);
    }
    /* Selected tagged records use the two negative row-placement slots,
     * regardless of which pool the preceding scan visited. */
    i = 0;
    do {
        u8 *base = (u8 *)&gTaggedRecordBase;
        rowSlot = -2;
        if (((Ovl11Work19C8View *)D_80074838)->field_19C8[i * 2] != -1) {
            firstSlot = (i == 0);
            recordAddress = (s32)(base + (((Ovl11Status99C8View *)D_8006C838)->field_99C8[i * 2 + 1] * 0xB8));
            if (firstSlot) {
                rowSlot = -1;
            }
            Ovl11ApplyRecordRowPlacement(recordAddress, rowSlot, 1);
        }
        i++;
    } while (i < 2);
}

Ovl11RecordPairSlot *Ovl11FindFreeRecordPairSlot(s32 poolKind) {
    Ovl11RecordPairSlot *freeSlot;
    s16 *pair;
    s16 *firstPair;
    u8 *base;
    u8 *flagBase;
    s32 i;
    s32 offset;
    s32 last;

    freeSlot = 0;
    if (poolKind == 1) {
        i = 0;
        base = (u8 *)gRecordPairSlots;
        firstPair = (s16 *)base;
        last = 1;
        flagBase = (u8 *)D_8006C838;
        for (; i < 2 && (i != last || *(u16 *)(flagBase + 0x44D0) != 0); i++) {
            offset = i * 4;
            if (*firstPair == -1) {
                freeSlot = (Ovl11RecordPairSlot *)(offset + (u32)base);
                break;
            }
            firstPair += 2;
        }
    } else {
        i = 2;
        base = (u8 *)D_8006C838;
        pair = (s16 *)(base + 0x99D0);
        for (; i < 4 && (i != 3 || *(u16 *)(base + 0x44D2) != 0); i++) {
            offset = i * 4;
            if (*pair == -1) {
                freeSlot = (Ovl11RecordPairSlot *)((u8 *)gRecordPairSlots + offset);
                break;
            }
            pair += 2;
        }
    }
    return freeSlot;
}

void Ovl11RestorePairedRecords(s32 poolOrSlot) {
    s16 selectedKind;
    s32 recordIndex;
    s32 recordFlags;
    s32 offset;
    s32 slotOffset;
    s32 z;
    s32 selectionAddress;
    s32 x, y;
    Ovl11D09E0Object *restored;
    u8 *selection;
    u8 *work;
    u8 *state;
    u8 *recordBase;
    u8 *selectionBase;
    u8 *b4Pool;

    /* The incoming pool selector becomes the slot cursor after dispatch.
     * The shared decode tail and one-iteration load block preserve the
     * original allocation; the stored selection and live slot are distinct. */
    if (poolOrSlot == 1) {
        poolOrSlot = 0;
        state = (u8 *) D_8006C838;
        work = D_80074838;
        recordBase = work + 0x1F0;
restore_b8_slot:
        selectionAddress = poolOrSlot * 4;
        selection = work + selectionAddress;
read_b8_selection:
        selectedKind = ((struct Ovl11D09E0SelectionView *) selection)->kind;
        selectionAddress = (s32)selection;
        selection = recordBase + selectedKind * 0xB8;
        do {
            recordFlags = *(s32 *)selection;
        } while (0);
        recordIndex = ((struct Ovl11D09E0SelectionView *) selectionAddress)->slot;
        if (recordFlags & 0x200) {
            if (recordFlags & 0x10000) {
                Ovl11InitOffsetRecordCopy((s32) selectedKind, (s32) recordIndex);
                poolOrSlot += 1;
            } else {
                Ovl11InitCode160OffsetCopy((s32) selectedKind, (s32) recordIndex);
                goto next_b8_slot;
            }
        } else {
next_b8_slot:
            poolOrSlot += 1;
        }
        if (poolOrSlot < 2) {
            selectionAddress = poolOrSlot * 4;
            if (poolOrSlot == 1) {
                if (((Ovl11Status99C8View *) state)->field_44D0 != 0) {
                    goto restore_b8_slot;
                }
                goto done;
            }
            goto restore_b8_slot;
        }
    } else {
        poolOrSlot = 2;
        selectionBase = D_80074838;
        recordBase = (u8 *) D_8006C838;
        b4Pool = recordBase + 0x7AB4;
        work = recordBase + 0x99D2;
        x = -0x640;
        y = -0xC8;
restore_b4_slot:
        slotOffset = poolOrSlot * 4;
        recordIndex = ((struct Ovl11D09E0SelectionView *) (selectionBase + slotOffset))->slot;
        offset = recordIndex * 0xB4;
        recordFlags = offset;
        if (((struct Ovl11PoolResetView *) (offset + (s32)recordBase))->records_7AB4[0].type == 0x10A) {
            Ovl11ClearSmallRecord(recordFlags + (s32) b4Pool);
            restored = (Ovl11D09E0Object *) Ovl11AllocateItemRecord(0x10A, (s32) recordIndex);
            if (*(s16 *)work == 2) {
                restored->flags |= 0x40000000;
            }
            restored->state = 3;
            z = 0x514;
            if (poolOrSlot == 2) {
                z = -0x44C;
            }
            restored->position.x = x;
            restored->position.y = y;
            restored->position.z = z;
        }
        poolOrSlot += 1;
        work += 4;
        if ((poolOrSlot < 4) && ((slotOffset = poolOrSlot * 4, (poolOrSlot != 3)) || (slotOffset = poolOrSlot * 4, (((Ovl11Status99C8View *) recordBase)->field_44D2 != 0)))) {
            goto restore_b4_slot;
        }
    }
done:
    return;
}

s32 Ovl11GetRecordPairSlotState(s32 slot) {
    if (slot == 1) {
        if (((Ovl11Status99C8View *)D_8006C838)->field_44D0 == 0) {
            return 2;
        }
    }
    if ((slot == 3) &&
        (((Ovl11Status99C8View *)D_8006C838)->field_44D2 == 0)) {
        return 2;
    }
    return ~((Ovl11Status99C8View *)D_8006C838)->field_99C8[slot * 2] != 0;
}

s32 *Ovl11FindFreeTaggedRecord(void) {
    s32 *record;
    s32 i;
    s32 limit;

    record = 0;
    limit = 10;
    if (gTaggedRecordPoolExpanded != 0) {
        limit = 20;
    }
    for (i = 0; i < limit; i++) {
        record = (s32 *)((u8 *)&gTaggedRecordBase + i * 0xB8);
        if (*(u16 *)record == 0 && !(*(s32 *)((char *)record + 0x34) & 0x02000000)) {
            break;
        }
    }
    if (i == limit) {
        return 0;
    }
    return record;
}

s32 *Ovl11FindFreeB4Record(void) {
    s32 *record;
    s32 i;
    s32 limit;
    char *base;

    base = (char *)&D_8006C838;
    record = 0;
    limit = 5;
    if (*(u16 *)(base + 0x44D2) != 0) {
        limit = 10;
    }
    for (i = 0; i < limit; i++) {
        record = (s32 *)((char *)&D_8006C838 + 0x7AB4 + i * 0xB4);
        if (*(u16 *)record == 0 && !(*(s32 *)((char *)record + 0x34) & 0x02000000)) {
            break;
        }
    }
    if (i == limit) {
        return 0;
    }
    return record;
}

s32 Ovl11GetTaggedRecordIndex(u8 *record) {
    s32 i;
    u8 *candidate;

    for (i = 0, candidate = (u8 *)&gTaggedRecordBase; i < 20; i++) {
        if (record == candidate) {
            return i;
        }
        candidate += 0xB8;
    }
    return -1;
}

void Ovl11SetRecordPairSlot(Ovl11RecordPairSlot *slot, s16 recordIndex, s16 linkedRecordIndex) {
    slot->recordIndex = recordIndex;
    slot->linkedRecordIndex = linkedRecordIndex;
}

void Ovl11ClearRecordPairSlot(Ovl11RecordPairSlot *slot) {
    slot->recordIndex = -1;
    slot->linkedRecordIndex = -1;
}
