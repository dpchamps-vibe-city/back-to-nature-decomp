/*
 * Selected-record counters driven by the game-flag protocol.
 *
 * The selected 0x1D4-byte event record supplies its unsigned +4 value to
 * threshold tests. Work +E76E counts stage updates (60/120/300 thresholds),
 * +E770 counts updates after the high-value threshold, +E772 caps low-value
 * observations at three, +E774 is a 30-update warmup and +E77A counts updates
 * without flag 4B. Selection and label index occupy +E778/+E776.
 *
 * These are invocation counts, not a proven real-time unit. The shared work
 * area and flags are also used outside this module; this is not reentrant.
 * Public contracts are in ovl11_record_flag_counters.h; grouping evidence is
 * in notes/file-groupings.md. Raw base-origin accesses retain the established
 * signed/unsigned views and the original address-emission boundaries.
 */

#include "common.h"
#include "psyq/memory.h"
#include "game_flags.h"
#include "ovl11_record_flag_counters.h"

void Ovl11InitRecordFlagCounters(void) {
    char *base;
    char *work;

    /* The shipped call has fill=0x34, length=0: do not turn it into a clear. */
    memset((u8 *)D_80074838 + 0x676C, 0x34, 0);
    base = (char *)&D_8006C838;
    work = base + 0x8000;
    *(s16 *)(work + 0x6776) = -1;
    SetGameFlag(0x1B, 1);
}

/* The snapshot starts at +E796, only halfword-aligned. */
typedef struct __attribute__((packed)) {
    s32 firstWord;
    s32 secondWord;
} Ovl11CounterSelectionSnapshot;

void Ovl11SelectCounterRecord(s16 recordIndex) {
    char *base;
    char *work;
    char *snapshotBase;
    char *snapshotWork;
    s16 selected;

    selected = recordIndex;
    base = (char *)&D_8006C838;
    work = base + 0x8000;
    *(u16 *)(work + 0x6778) = selected;
    if (selected == 3) {
        *(u16 *)(work + 0x6776) = 0;
    } else if (selected == 7) {
        *(u16 *)(work + 0x6776) = 1;
    } else if (selected == 9) {
        *(u16 *)(work + 0x6776) = 2;
    } else if (selected == 0xE) {
        *(u16 *)(work + 0x6776) = 3;
    } else {
        *(u16 *)(work + 0x6776) = 4;
    }
    snapshotBase = (char *)&D_8006C838;
    snapshotWork = snapshotBase + 0x8000;
    *(u16 *)(snapshotWork + 0x676C) = 0;
    *(Ovl11CounterSelectionSnapshot *)(snapshotWork + 0x6796) =
        *(Ovl11CounterSelectionSnapshot *)(snapshotBase + 0x44B8);
}

void Ovl11ResetRecordThresholdCount(void) {
    char *base;
    u16 firstHalfword;
    u16 secondHalfword;

    base = (char *)&D_8006C838;
    firstHalfword = *(u16 *)(base + 0x44BA);
    secondHalfword = *(u16 *)(base + 0x44BC);
    base += 0x8000;
    *(u16 *)(base + 0x5BDC) = firstHalfword;
    *(u16 *)(base + 0x5BDE) = secondHalfword;
    SetGameFlag(0x4E, 0);
    *(u16 *)(base + 0x6770) = 0;
}

/* These two callees ignore the original caller's material pointer arguments.
 * Preserve that ABI evidence without inventing a consumed parameter type. */
void Ovl11ConsumeRecordActivityFlags();
void Ovl11StepRecordStageCount();
void Ovl11StepRecordThresholdCount(struct_80076220 *record);
void Ovl11StepRecordLowValueCount(struct_80076220 *record);

void Ovl11UpdateRecordFlagCounters(void) {
    s32 shifted;
    s32 product;
    u32 sum;
    s32 upper;
    s16 recordIndex;
    u32 base;
    s32 active;
    struct_80076220 *record;

    active = GetGameFlag(0x41);
    if (active != 0) {
        base = (s32)&D_8006C838;
        do {
            upper = 0x8000;
            base += upper;
        } while (0);
        recordIndex = *(s16 *)(base + 0x6778);
        base = (s32)&gObjectEventSlotBase;
        /* Preserve the staged 0x1D4-byte record-offset calculation. */
        product = (u32)recordIndex << 3;
        product -= recordIndex;
        shifted = (u32)product << 2;
        sum = shifted + recordIndex;
        shifted = (u32)sum << 2;
        sum = shifted + recordIndex;
        shifted = (u32)sum << 2;
        record = (struct_80076220 *)(base + shifted);
        Ovl11ConsumeRecordActivityFlags(record);
        Ovl11StepRecordThresholdCount(record);
        Ovl11StepRecordLowValueCount(record);
        if (GetGameFlag(0x4F) != 0) {
            Ovl11StepRecordStageCount(&D_8007A3F0);
        }
    }
}

s32 Ovl11GetCounterRecordLabel(void) {
    s32 **labels = &gOvl11CounterRecordLabels[0];
    s16 *work = (s16 *)D_8006C838;

    return labels[work[0x73BB]];
}

void Ovl11StepRecordThresholdCount(struct_80076220 *record) {
    s32 address;
    unsigned char *work;

    if (GetGameFlag(0x4F) != 1) {
        address = (s32)&D_8006C838;
        work = (unsigned char *)(address + 0x8000);
        if (*(s16 *)(work + 0x6774) < 0x1E) {
            *(u16 *)(work + 0x6774) += 1;
            return;
        }
        if (GetGameFlag(0x4E) == 1) {
            *(u16 *)(work + 0x6770) += 1;
            return;
        }
        if (record->unk4 > 0xFDE7U) {
            SetGameFlag(0x4E, 1);
        }
    }
}

void Ovl11StepRecordStageCount(void) {
    s32 address;
    unsigned char *work;
    u16 stageFlag;

    address = (s32)&D_8006C838;
    work = (unsigned char *)(address + 0x8000);
    if (*(u16 *)(work + 0x676E) < 0x12D) {
        SetGameFlag(0x50, 0);
        SetGameFlag(0x51, 0);
        SetGameFlag(0x52, 0);
        if (*(u16 *)(work + 0x676E) < 0x12C) {
            *(u16 *)(work + 0x676E) = *(u16 *)(work + 0x676E) + 1;
        }
        if (*(u16 *)(work + 0x676E) >= 0x78) {
            *(s16 *)(work + 0x677C) = 2;
            stageFlag = 0x52;
            goto set_stage_flag;
        }
        if (*(u16 *)(work + 0x676E) >= 0x3C) {
            *(s16 *)(work + 0x677C) = 1;
            stageFlag = 0x51;
set_stage_flag:
            SetGameFlag(stageFlag, 1);
            return;
        }
        SetGameFlag(0x50, 1);
    }
}

void Ovl11ConsumeRecordActivityFlags(void) {
    char *work;
    char *resetWork;

    if (GetGameFlag(0x4B) == 0) {
        work = (char *)&D_8006C838;
        work = work + 0x8000;
        *(u16 *)(work + 0x677A) += 1;
    } else {
        resetWork = (char *)&D_8006C838;
        resetWork = resetWork + 0x8000;
        *(u16 *)(resetWork + 0x677A) = 0;
    }
    SetGameFlag(0x4B, 0);
    SetGameFlag(0x4C, 0);
}

void Ovl11StepRecordLowValueCount(struct_80076220 *record) {
    s32 address;
    unsigned char *work;

    if (record->unk4 < 0x3A99) {
        if (GetGameFlag(0x2B) == 0) {
            SetGameFlag(0x4D, 1);
            SetGameFlag(0xAF, 1);
            address = (s32)&D_8006C838;
            work = (unsigned char *)(address + 0x8000);
            if (*(u16 *)(work + 0x6772) < 3) {
                *(u16 *)(work + 0x6772) = *(u16 *)(work + 0x6772) + 1;
            }
        }
    }
}
