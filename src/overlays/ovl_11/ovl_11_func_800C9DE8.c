#include "common.h"
#include "game_types.h"
#include "ovl11_triple_record_table.h"

s32 ovl_11_func_800C9DE8(s32 arg0) {
    if (arg0 == 0) {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&Ovl11Items_80071AC0, 8, ((Ovl11TripleRecord *)&Ovl11Items_80071AC0 - 9), 0);
    } else {
        Ovl11ExchangeTripleRecordEndpoint((Ovl11TripleRecord *)&Ovl11Items_80071AC0, 8, ((Ovl11TripleRecord *)&Ovl11Items_80071AC0 - 9), 1);
    }
    return 1;
}
