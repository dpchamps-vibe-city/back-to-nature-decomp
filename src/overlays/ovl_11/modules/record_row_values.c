/*
 * Row-selected record values, original ovl_11:800F508C-800F5210.
 * The pool visitor and row writer share the flag-priority helper and its
 * four signed-halfword triples. Work owns all nineteen lists and counts.
 * The existing static-chain capture and block-local auto declaration retain
 * the original nested-function call convention; neither is a new workaround.
 * Contract: ovl11_record_row_values.h. Evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_record_row_values.h"

void Ovl11AssignRecordRowValues(s16 row, s16 count, Ovl11DispatchRecord *records);

void Ovl11AssignPoolRowValues(s16 row) {
    s16 *counts;
    s16 count;
    s32 listsRemaining;
    void **lists;
    char *work;
    void *records;

    work = (char *)&D_8006C838;
    lists = (void **)(work + 0xDD8C);
    counts = (s16 *)(work + 0xDDD8);
    listsRemaining = 0x12;
    do {
        records = *lists;
        lists += 1;
        count = *counts;
        counts += 1;
        listsRemaining -= 1;
        Ovl11AssignRecordRowValues(row, count, records);
    } while (listsRemaining >= 0);
}

s16 Ovl11SelectRecordRowValue(u16 flags, s16 first, s16 second, s16 third) {
    s32 chainSlot[2];
    CAPTURE_PREV_RET(staticChain);

    chainSlot[0] = staticChain;
    if (flags & 0x2000) {
        return first;
    }
    if (flags & 0x800) {
        return second;
    }
    if (flags & 0x1000) {
        return third;
    }
    return 0;
}

void Ovl11AssignRecordRowValues(s16 row, s16 count, Ovl11DispatchRecord *records) {
    auto s16 selectRowValue(u16 flags, s16 first, s16 second, s16 third) __asm__("Ovl11SelectRecordRowValue");
    s32 value;
    s32 i;

    for (i = 0; i < count; i++) {
        s32 offset = row * 6;
        value = selectRowValue(records[i].flags,
            *(s16 *)(gOvl11RecordRowValueTriples + offset),
            *(s16 *)(gOvl11RecordRowValueTriples + offset + 2),
            *(s16 *)(gOvl11RecordRowValueTriples + offset + 4));
        if (value != 0) {
            records[i].kind = value;
        }
    }
}
