/*
 * Fixed primary-object item lists, original ovl_11:800C9D98-800C9E90.
 * The stack list begins at object +0x90 and the secondary list at +0xC0;
 * each holds eight six-byte records. Their selected records live at +0x84
 * and +0x8A respectively. Storage belongs to the shared object, not this TU.
 * See ovl11_item_record_lists.h for exchange direction and return semantics.
 */
#include "common.h"
#include "ovl11_item_record_lists.h"
#include "ovl11_triple_record_table.h"

void Ovl11CompactSecondaryItemList(void) {
    Ovl11CompactTripleRecords((Ovl11TripleRecord *)&gPrimaryObjectSecondaryItemList, 8);
}

void Ovl11CompactItemStackList(void) {
    Ovl11CompactTripleRecords((Ovl11TripleRecord *)&gItemStackSlotBase, 8);
}

s32 Ovl11ExchangeSecondaryItemList(s32 reverse) {
    if (reverse == 0) {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&gPrimaryObjectSecondaryItemList, 8,
            (Ovl11TripleRecord *)&gPrimaryObjectSecondaryItemList - 9, 0);
    } else {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&gPrimaryObjectSecondaryItemList, 8,
            (Ovl11TripleRecord *)&gPrimaryObjectSecondaryItemList - 9, 1);
    }
    return 1;
}

s32 Ovl11ExchangeItemStackList(s32 reverse) {
    /* The subtraction precedes the cast: three s32 words, not three records. */
    if (reverse == 0) {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&gItemStackSlotBase, 8,
            (Ovl11TripleRecord *)(&gItemStackSlotBase - 3), 0);
    } else {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&gItemStackSlotBase, 8,
            (Ovl11TripleRecord *)(&gItemStackSlotBase - 3), 1);
    }
    return 1;
}
