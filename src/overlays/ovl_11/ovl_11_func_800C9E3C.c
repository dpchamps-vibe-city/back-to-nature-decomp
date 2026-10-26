#include "common.h"
#include "game_types.h"
#include "ovl11_triple_record_table.h"

s32 ovl_11_func_800C9E3C(s32 arg0) {
    if (arg0 == 0) {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *) &gItemStackSlotBase, 8, (Ovl11TripleRecord *) ((&gItemStackSlotBase - 3)), 0);
    } else {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *) &gItemStackSlotBase, 8, (Ovl11TripleRecord *) ((&gItemStackSlotBase - 3)), 1);
    }
    return 1;
}
