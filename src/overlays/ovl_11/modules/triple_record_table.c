/*
 * Caller-owned six-byte record tables: stable compaction and exchange with
 * one external record. No global state is read or owned by this module.
 * See ovl11_triple_record_table.h for the endpoint/capacity preconditions.
 * Original entries: ovl_11:800CADBC and 800CAE64, in that order.
 */

#include "common.h"
#include "ovl11_triple_record_table.h"
#include "psyq/memory.h"

s32 Ovl11CompactTripleRecords(Ovl11TripleRecord *records, s32 count) {
    s32 hole;
    s32 source;

    for (hole = 0; hole < count; hole++) {
        if (records[hole].key == 0) {
            for (source = hole + 1; source < count; source++) {
                if (records[source].key != 0) {
                    records[hole].key = records[source].key;
                    records[hole].payload0 = records[source].payload0;
                    records[hole].payload1 = records[source].payload1;
                    records[source].key = 0;
                    records[source].payload0 = 0;
                    records[source].payload1 = 0;
                    break;
                }
            }
        }
    }
    return 0;
}

s32 Ovl11ExchangeTripleRecordEndpoint(Ovl11TripleRecord *records, s32 count,
                       Ovl11TripleRecord *external, s32 direction) {
    Ovl11TripleRecord *end;
    Ovl11TripleRecord *last;
    Ovl11TripleRecord saved;
    s32 lastIndex;

    if ((records->key != 0) || (external->key != 0)) {
        switch (direction) {
        case 0:
            saved = *records;
            records->key = 0;
            records->payload0 = 0;
            records->payload1 = 0;
            Ovl11CompactTripleRecords(records, count);
            end = (Ovl11TripleRecord *) (count * 6 + (s32) records);
            end[-1] = *external;
            Ovl11CompactTripleRecords(records, count);
            *external = saved;
            break;
        case 1:
            lastIndex = count - 1;
            while ((lastIndex >= 0) && (records[lastIndex].key == 0)) {
                lastIndex -= 1;
            }
            last = (Ovl11TripleRecord *) (lastIndex * 6 + (s32) records);
            saved = *last;
            last->key = 0;
            last->payload0 = 0;
            last->payload1 = 0;
            memmove((u8 *) (records + 1), (u8 *) records, lastIndex * 6);
            *records = *external;
            *external = saved;
            Ovl11CompactTripleRecords(records, count);
            break;
        }
        return -1;
    }
    return -1;
}
