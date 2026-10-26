#include "common.h"
#include "ovl11_record_pair_slots.h"

void ovl_11_func_800E2BE8(s32 arg0) {
    s32 i;
    s16 v;

    for (i = 0; i < 10; i++) {
        v = *(s16 *)(arg0 + 0x30);
        if (v != 4 && v != 1) {
            Ovl11ApplyRecordRowPlacement(arg0, (s16)i, 0);
        }
    }
}
