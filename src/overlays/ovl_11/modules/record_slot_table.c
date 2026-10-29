/*
 * Five-slot record queue and tracked counters, ovl_11:DB9A4-DBFA0.
 * Slots at shared work +7A78 contain six halfwords; -1 in the first means
 * free. The word immediately before them is a dispatch phase, not a count.
 * Initialisation also seeds the counters advanced by this unit. No storage
 * is allocated here; all queue/counter state remains in the saved work areas.
 */
#include "common.h"
#include "ovl11_record_slot_table.h"
#include "psyq/memory.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "game_flags.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_interaction_state_access.h"
#include "ovl11_condition_date_records.h"

s32 Ovl11ResetStaleRecordSlot(s32 unused);
s32 Ovl11ServiceRecordSlot(s32 tag);
void Ovl11DropFirstRecordSlot(void);
s16 *Ovl11FindFreeRecordSlot(s32 tag, s32 rejectExisting, s32 scanAll);

void Ovl11InitRecordSlotTable(void) {
    s32 i;
    u8 *counter;
    u8 *upperWork;
    u8 *secondaryWork;
    u8 *clearWork;
    u8 *seedWork;
    u8 idle;
    u8 secondaryIdle;

    memset(&gRecordSlotPhase, 0, 0x40);
    for (i = 0; i < 5; i++) {
        *(s16 *)((u8 *)&gRecordSlotPhase + 4 + i * 0xC) = -1;
    }
    counter = (u8 *)&D_8006C838;
    *(s32 *)(counter + 0x30) = 0;
    idle = 0xFF;
    i = 0x7F;
    counter += 0xE6BF;
    do {
        *counter = idle;
        counter -= 1;
        i -= 1;
    } while (i >= 0);

    secondaryIdle = 0xFF;
    for (i = 7; i >= 0; i--) {
        secondaryWork = (u8 *)&D_8006C838;
        secondaryWork[0xE6C0 + i] = secondaryIdle;
    }
    for (i = 0x7F; i >= 0; i--) {
        clearWork = (u8 *)&D_8006C838;
        clearWork[0xE6C8 + i] = 0;
    }
    seedWork = (u8 *)&D_8006C838;
    upperWork = seedWork + 0x8000;
    *(s8 *)(upperWork + 0x6641) = 1;
    *(s8 *)(upperWork + 0x6646) = 0x1E;
    *(s8 *)(upperWork + 0x664C) = 0x1F;
    *(s8 *)(upperWork + 0x6650) = 0;
    *(s8 *)(upperWork + 0x6655) = 0x1C;
    *(s8 *)(upperWork + 0x6656) = 0;
    *(s8 *)(upperWork + 0x6658) = 0x76;
    *(s8 *)(upperWork + 0x6659) = 0x1C;
    *(s8 *)(upperWork + 0x666C) = 0;
}

void Ovl11ServiceActiveRecordSlot(void) {
    u8 *work;
    u8 *flagWork;
    s16 tag;

    work = (u8 *)D_8006C838;
    if (!(*(s32 *)(work + 0x10) & 0x1000)) {
        tag = *(s16 *)(work + 0x7A78);
        *(void **)(work + 0x30) = (void *)(work + 0x7A78);
        if (tag == -1) {
            SetGameFlag(2U, 0U);
            if ((Ovl11GetInteractionId() == 0) && (*(s16 *)(work + 0x51EE) == 0)) {
                *(s32 *)(work + 0x5234) &= 0xFF7FFFFF;
            }
            flagWork = (u8 *)D_8006C838;
            *(s32 *)(flagWork + 0x4450) &= ~0x1000;
            return;
        }
        if ((tag != 0) || (GetGameFlag(2U) != 0) || (Ovl11ResetStaleRecordSlot(0) == 0)) {
            Ovl11ServiceRecordSlot((s32)tag);
        }
    }
}

s32 Ovl11RecordSlotTagMatches(s32 tag) {
    s16 *work = (s16 *)D_8006C838;
    s32 i = 0;

    for (; i < 5; i++) {
        if (work[0x3D3C + i * 6] != -1 && work[0x3D3C + i * 6] == tag) {
            return 1;
        }
    }
    return 0;
}

void Ovl11ReserveFirstRecordSlot(s32 tag) {
    Ovl11FindFreeRecordSlot(tag, 0, 0);
}

s16 *Ovl11ReserveFirstRecordSlotIfAbsent(s32 tag) {
    return Ovl11FindFreeRecordSlot(tag, 1, 0);
}

s16 *Ovl11ReserveAnyRecordSlot(s32 tag) {
    return Ovl11FindFreeRecordSlot(tag, 0, 1);
}

void Ovl11AdvanceTrackedRecordCounters(void) {
    s32 i;

    for (i = 0; i < 0x80; i++) {
        if (gTrackedRecordCounters[i] != 0xFF) {
            gTrackedRecordCounters[i]++;
            gTrackedRecordCounters[i] = gTrackedRecordCounters[i] >= 0x79 ? 0x78 : gTrackedRecordCounters[i];
        }
    }
    if (Ovl11FindCalendarDate(
            ((struct Ovl11TimerState *)D_8006C838)->field_44BA,
            ((struct Ovl11TimerState *)D_8006C838)->field_44BC) != 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        if (Ovl11ClassifyConditionDateSlot(i) == 2) {
            return;
        }
    }
    if (Ovl11ClassifyConditionDateSlot(-1) != 2 &&
        (u32)(((struct Ovl11TimerState *)D_8006C838)->field_E4C8 - 3) >= 2U) {
        for (i = 0; i < 8; i++) {
            if (gSecondaryRecordCounters[i] != 0xFF) {
                gSecondaryRecordCounters[i]++;
                gSecondaryRecordCounters[i] = gSecondaryRecordCounters[i] >= 0x79 ? 0x78 : gSecondaryRecordCounters[i];
            }
        }
    }
}

/* tag is a duplicate-test key, NOT the value installed in a new slot.
 * Reuse the scan counter for the backwards payload clear, as in the original. */
s16 *Ovl11FindFreeRecordSlot(s32 tag, s32 rejectExisting, s32 scanAll) {
    s16 *record;
    s16 *clear;
    s32 limit;
    s32 index;
    void *payload;

    if ((rejectExisting != 1) || (Ovl11RecordSlotTagMatches(tag) != rejectExisting)) {
        limit = 1;
        if (scanAll == 1) {
            limit = 5;
        }
        index = 0;
        if (limit != 0) {
            s16 freeTag = -1;
            s16 *work = (s16 *)D_8006C838;
            payload = (void *)(work + 0x3D3D);
            record = work + 0x3D3C;
        scan:
            index += 1;
            if (*record == freeTag) {
                *record = 0;
                index = 4;
                clear = (s16 *)((u8 *)payload + 8);
                do {
                    *clear = 0;
                    index -= 1;
                    clear -= 1;
                } while (index >= 0);
                return record;
            }
            payload = (void *)((u8 *)payload + 0xC);
            record += 6;
            if (index >= limit) {
                goto full;
            }
            goto scan;
        }
    full:
        return NULL;
    }
    return NULL;
}

s32 Ovl11ServiceRecordSlot(s32 tag) {
    Ovl11RecordSlotHandler *slot;
    Ovl11RecordSlotHandler *table;

    table = gOvl11RecordSlotHandlers;
    slot = &table[tag];
    if ((*slot)(slot) != 1) {
        SetGameFlag(2, 1);
        return 0;
    }
    SetGameFlag(2, 0);
    gRecordSlotPhase = 0;
    Ovl11DropFirstRecordSlot();
    return 1;
}

/* Shift unconditionally, not hole compaction. Packed copies retain the
 * original unaligned access contract. The old tail payload is left intact. */
void Ovl11DropFirstRecordSlot(void) {
    char *work = (char *)&D_8006C838;
    Ovl11RecordSlotCopy *destination = (Ovl11RecordSlotCopy *)(work + 0x7A78);
    s32 i;

    for (i = 0; i < 4; i++) {
        *destination = destination[1];
        destination++;
    }
    ((Ovl11RecordSlotTailView *)&D_8006C838)->lastTag = -1;
}

s32 Ovl11ResetStaleRecordSlot(s32 unused) {
    char *work;
    char *farWork;
    s32 *active;

    work = (char *)&D_8006C838;
    farWork = (char *)&D_8007AFF0;
    active = ((Ovl11ActiveRecordSlotView *)work)->activeRecord;
    if (*(s16 *)((char *)active + 4) == *(s16 *)(farWork + 0x25476)) {
        return 0;
    }
    SetGameFlag(2, 0);
    *(s32 *)(work + 0x7A74) = 0;
    Ovl11DropFirstRecordSlot();
    return 1;
}

s32 Ovl11HasQueuedRecordSlots(void) {
    s16 *work = (s16 *)D_8006C838;
    s32 i = 1;

    for (; i < 5; i++) {
        if (work[0x3D3C + i * 6] != -1) {
            return 1;
        }
    }
    return 0;
}
