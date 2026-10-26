/*
 * Read-only flag query over the twenty 0xB8-byte records at D_800749F4
 * and ten 0xB4-byte records at D_800742EC. Pool selection for the first
 * array comes from the parallel state words at D_8006C838+0x81F0.
 *
 * The predicate is genuinely nested: its original entry spills the GCC
 * static chain. Keeping that scope emits both original functions in order
 * without a separate capture or register constraint. Pool storage is shared
 * with other modules; this query does not claim ownership of it.
 * Original entries: ovl_11:800CFAD0/800CFB20; see file-groupings.md.
 */

#include "common.h"
#include "ovl11_record_flag_query.h"

typedef struct {
    /* 0x00 */ u16 field0;
    /* 0x02 */ char reserved[0x34 - 0x02];
    /* 0x34 */ s32 flags;
} Ovl11RecordFlagView;

s32 Ovl11AnyRecordFlagMatch(s32 poolMask, s32 selectionMask) {
    s32 Ovl11TestRecordFlagSelection(Ovl11RecordFlagView *record, s32 mask) {
        s32 flags;
        s32 maskedFlag;

        flags = record->flags;
        maskedFlag = flags & 0x80000;
        if ((flags & 0x100) != 0) {
            if ((mask & 1) != 0) {
                /* Deliberately compare the mask result to 1, not nonzero.
                 * This arm cannot accept a record; preserve the original. */
                if (maskedFlag == 1) {
                    return 1;
                }
            }
            if ((mask & 2) != 0) {
                if (maskedFlag == 0) {
                    return 1;
                }
            }
        }
        return 0;
    }
    unsigned char *state;
    unsigned char *work;
    s32 *record;
    s32 index;

    if (poolMask & 3) {
        index = 0;
        record = &gTaggedRecordBase;
        work = (unsigned char *) &D_8006C838;
        state = work + 0x81F0;
scan_primary:
        if ((*(s32 *) state & 0x10000) ? (poolMask & 2) : (poolMask & 1)) {
            if (Ovl11TestRecordFlagSelection((Ovl11RecordFlagView *) record, selectionMask)) {
                return 1;
            }
        }
        record += 0x2E;
        index += 1;
        state += 0xB8;
        if (index < 0x14) {
            goto scan_primary;
        }
    }
    index = 0;
    if (poolMask & 4) {
        state = (unsigned char *) &D_800742EC;
scan_secondary:
        if (Ovl11TestRecordFlagSelection((Ovl11RecordFlagView *) state, selectionMask)) {
            return 1;
        }
        index += 1;
        state += 0xB4;
        if (index < 0xA) {
            goto scan_secondary;
        }
    }
    return 0;
}
