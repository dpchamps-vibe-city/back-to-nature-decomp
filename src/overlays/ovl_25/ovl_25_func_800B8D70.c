#include "common.h"

void ovl_25_func_800B8DF4(s16 *arg0);

void ovl_25_func_800B8D70(void) {
    s16 *record;
    u8 *base;
    s32 i;
    s32 offset;

    offset = 0;
    record = (s16 *) &D_800BFE44_view;
    base = (u8 *) record + 0x10;
    i = 5;
    do {
        if (record[0xA] > 0) {
            record[0xA]--;
            if (record[0xA] < 0) {
                record[0xA] = 0;
            }
        }
        ovl_25_func_800B8DF4((s16 *) (offset + (u32) base));
        offset += 0x78;
        i--;
        record += 0x3C;
    } while (i >= 0);
}
