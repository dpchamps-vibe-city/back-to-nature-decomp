/*
 * record_pool_refresh — reconstructed translation unit.
 *
 * Refresh dispatch-record enable bits from work flags and byte lists, with
 * keyed lookup and mode-selected coordinate rules for the second list.
 *
 * Original ovl_11:800F5404-800F581C. The status/byte-list refresh calls
 * the keyed flag writer and, in mode 2, the private coordinate-rule pass.
 * Work owns the lists; tables remain extracted overlay data. All helpers
 * retain external linkage. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_record_pool_refresh.h"
#include "ovl11_pair_slot_records.h"

void Ovl11SetDispatchRecordEnabledByMask(s16 arg0, Ovl11DispatchRecord arg1[], s32 arg2, s32 arg3, s32 arg4);
void Ovl11ApplyDispatchRecordCoordinateRules(void);

void Ovl11RefreshDispatchRecordPools(void) {
    u8 *enabled;
    char *far_base;
    Ovl11DispatchRecord *entries;
    s16 count;
    s32 flags;
    s32 i;
    s32 j;

    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[0];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[0];
    flags = ((struct Ovl11FlagRefreshState *) D_8006C838)->status;
    Ovl11SetDispatchRecordEnabledByMask(0x83, entries, count, flags, 0x400);
    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[1];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[1];
    Ovl11SetDispatchRecordEnabledByMask(0x71, entries, count, flags, 2);
    Ovl11SetDispatchRecordEnabledByMask(0x188, entries, count,
        ((struct Ovl11FlagRefreshState *) D_8006C838)->status2, 0x40);
    flags = ((struct Ovl11FlagRefreshState *) D_8006C838)->flags;
    Ovl11SetDispatchRecordEnabledByMask(0x72, entries, count, flags, 1);
    Ovl11SetDispatchRecordEnabledByMask(0x73, entries, count, flags, 2);
    Ovl11SetDispatchRecordEnabledByMask(0x74, entries, count, flags, 4);
    Ovl11SetDispatchRecordEnabledByMask(0x75, entries, count, flags, 8);
    Ovl11SetDispatchRecordEnabledByMask(0x76, entries, count, flags, 0x10);
    Ovl11SetDispatchRecordEnabledByMask(0x77, entries, count, flags, 0x20);
    Ovl11SetDispatchRecordEnabledByMask(0x78, entries, count, flags, 0x40);
    Ovl11SetDispatchRecordEnabledByMask(0x79, entries, count, flags, 0x80);

    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[2];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[2];
    enabled = ((struct Ovl11FlagRefreshState *) D_8006C838)->enabled;
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            entries[i].flags |= 1;
        } else {
            entries[i].flags &= 0xFFFE;
        }
    }
    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[3];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[3];
    enabled = ((struct Ovl11FlagRefreshState *) D_8006C838)->enabled2;
    for (j = 0; j < count; j++) {
        if (enabled[j] != 0) {
            entries[j].flags |= 1;
        } else {
            entries[j].flags &= 0xFFFE;
        }
    }
    Ovl11RefreshPairSlotRecords();
    far_base = (char *) &D_8007AFF0;
    if (*(s16 *) (far_base + 0x25476) == 2) {
        Ovl11ApplyDispatchRecordCoordinateRules();
    }
}

void Ovl11SetDispatchRecordEnabledByMask(s16 kind, Ovl11DispatchRecord entries[], s32 count, s32 flags, s32 mask) {
    Ovl11DispatchRecord *record;

    record = Ovl11FindDispatchRecordByKind(kind, entries, count);
    if (record != 0) {
        if (flags & mask) {
            record->flags |= 1;
        } else {
            record->flags &= 0xFFFE;
        }
    }
}

Ovl11DispatchRecord *Ovl11FindDispatchRecordByKind(s16 kind, Ovl11DispatchRecord entries[], s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (entries[i].kind == kind)
            return &entries[i];
    }
    return 0;
}

void Ovl11ApplyDispatchRecordCoordinateRules(void) {
    char *base;
    Ovl11DispatchRecord *record;
    struct Ovl11RecordCoordinateRule *rules;
    s32 ruleCount;
    s32 recordCount;
    s32 recordIndex;
    s32 selector;

    base = (char *)&D_8006C838;
    record = *(Ovl11DispatchRecord **)(base + 0xDD90);
    recordCount = *(s16 *)(base + 0xDDDA);
    selector = *(u16 *)(base + 0x44CE);

    switch (selector) {
    case 0:
    case 3:
    default:
        rules = gOvl11RecordCoordinateRulesDefault;
        ruleCount = 2;
        break;
    case 1:
        rules = gOvl11RecordCoordinateRules1;
        ruleCount = 10;
        break;
    case 2:
        rules = gOvl11RecordCoordinateRules2;
        ruleCount = 10;
        break;
    }

    /* Retain separate next-index and pointer steps in this nested scan. */
    for (recordIndex = 0; recordIndex < recordCount;) {
        s32 i;
        s32 nextIndex = recordIndex + 1;
        for (i = 0; i < ruleCount; i++) {
            if (record->kind != rules[i].kind) {
                continue;
            }
            record->x = rules[i].x;
            record->y = rules[i].y;
            record->z = rules[i].z;
            break;
        }
        record = (Ovl11DispatchRecord *)((char *)record + 0x18);
        recordIndex = nextIndex;
    }
}
