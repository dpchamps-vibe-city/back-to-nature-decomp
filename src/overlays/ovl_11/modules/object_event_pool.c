/*
 * Lifecycle and counter maintenance for 37 shared object-event records.
 * Each slot is 0x1D4 bytes; signed ID -1 marks it free. Allocation raises
 * flag 0x8000, consumed by the separate event-script update module.
 * Original order: ovl_11:800C1BA0..800C1F94 exclusive.
 * Public contracts: ovl11_object_event_pool.h. Grouping: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "random.h"
#include "game_flags.h"
#include "ovl11_object_event_pool.h"

void ovl_11_func_800F0C70(void);
void ovl_11_func_80107DD0(s16 *timer);
s32 ovl_11_func_800C3548(s32 slotIndex);

/* Internal callers only, but original external linkage is retained. The
 * initializer's s32 argument is an ADDRESS, not a numeric slot index. */
void Ovl11InitEventSlot(s32 slotAddress);
void Ovl11AllocateEventSlot(s32 id);

void Ovl11AllocateAllEventSlots(void) {
    s32 id;

    id = 0;
    do {
        Ovl11AllocateEventSlot(id);
        id += 1;
    } while (id < 0x25);
    ovl_11_func_800F0C70();
}

void Ovl11ClearEventSlotTimeOrigins(void) {
    u32 index;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    for (index = 0; index < 37; index++) {
        slot->unkA = 0;
        slot++;
    }
}

void Ovl11ResetAllEventSlots(void) {
    s32 remaining;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    remaining = 0x24;
    do {
        Ovl11InitEventSlot((s32) slot);
        ovl_11_func_80107DD0((s16 *) &slot->unk30[0xB0]);
        remaining -= 1;
        slot += 1;
    } while (remaining >= 0);
    ovl_11_func_800F0C70();
}

void Ovl11InitEventSlot(s32 slotAddress) {
    memset(slotAddress, 0, 0x1D4);
    *(s16 *)slotAddress = -1;
}

void Ovl11ClearEventSlotScheduleChanges(void) {
    s32 remaining;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    remaining = 0x24;
    do {
        slot->unk1E &= ~0x10;
        remaining--;
        slot++;
    } while (remaining >= 0);
}

/* First-free scan. A full pool silently leaves state unchanged. Stored IDs
 * narrow to s16; initial counters depend on SLOT index, not the requested ID. */
void Ovl11AllocateEventSlot(s32 id) {
    s32 slotIndex;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    slotIndex = 0;
search_slot:
    if ((*(s16 *) ((u8 *) slot + 0)) == -1) {
        Ovl11InitEventSlot((s32) slot);
        (*(s16 *) ((u8 *) slot + 0)) = id;
        slot->unk1E |= 0x8000;
        ovl_11_func_80107DD0((s16 *) &slot->unk30[0xB0]);
        if (ovl_11_func_800C3548((s16) slotIndex) == 1) {
            slot->unk4 = 0;
            slot->unk2 = 0;
            return;
        }
        slot->unk4 = 0;
        slot->unk2 = 0x32;
        return;
    }
    slotIndex += 1;
    slot += 1;
    if (slotIndex >= 0x25) {
        return;
    }
    goto search_slot;
}

void Ovl11UpdateEventSlotCounters(void) {
    u32 slotIndex;
    struct_80076220 *slot;
    s32 value;
    s32 count;
    s32 kind;
    s32 one;
    u8 *work;

    slot = &gObjectEventSlotBase;
    slotIndex = 0;
    one = 1;
    /* Reuse the shared-state anchor of the first record for the selected
     * slot word. The 0x19E8 offset is part of the established split-base view. */
    work = (u8 *)slot - 0x19E8;
    for (; slotIndex < 0x25; slotIndex++, slot++) {
        kind = ovl_11_func_800C3548((s16)slotIndex);
        value = slot->unk4;
        count = (s16)slot->unk2;
        if (slot->unk1E & 0x400) {
            if (kind == one) {
                value += 0xC8;
            } else {
                count++;
            }
        }
        switch (slotIndex) {
        case 0x19:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x23:
            break;
        default:
            if (!(slot->unk1E & 0x600) && Rand(10) == 0) {
                if (kind == one) {
                    value -= 0xC8;
                } else {
                    count--;
                }
            }
            break;
        }
        if (value > 0xFFFF) value = 0xFFFF;
        if (value < 0) value = 0;
        if (count > 0xFF) count = 0xFF;
        if (count < 0) count = 0;
        slot->unk4 = value;
        slot->unk2 = count;
        if (slotIndex == *(s16 *)(work + 0x6778)) {
            if (slot->unk1E & 0x200) {
                SetGameFlag(0x4B, 1);
            } else {
                SetGameFlag(0x4B, 0);
            }
        }
        slot->unk1E &= ~0x600;
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_9F68 >= 0x5DD) {
        SetGameFlag(0xE0, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_A6B8 >= 0x5DD) {
        SetGameFlag(0xE1, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_AA60 >= 0x5DD) {
        SetGameFlag(0xE2, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_B384 >= 0x5DD) {
        SetGameFlag(0xE3, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_BE7C >= 0x5DD) {
        SetGameFlag(0xE4, 1);
    }
}
