#include "common.h"
#include "ovl11_triple_record_table.h"

extern u8 D_80071AC0[];

void ovl_11_func_800C9D98(void) {
    Ovl11CompactTripleRecords(((s32)(&D_80071AC0)), 8);
}
