/* Count or select eligible records in the ten-B4 and twenty-B8 shared pools.
 * This query owns no state and performs no allocation or RNG calls. The
 * public ordinal/filter and ambiguous-zero contract is documented in
 * ovl11_record_family_lookup.h. Keep the B8 byte-stride view: its tail fields
 * are not represented by the smaller initializer views used elsewhere. */
#include "common.h"
#include "ovl11_record_family_lookup.h"

s32 Ovl11FindFamilyRecordIndex(s16 family, s16 filter, s16 ordinal) {
    s32 count = 0;
    s16 result = 0;
    s32 index;
    u8 *recordBase;

    switch (family) {
    case 0:
        for (index = 0; index < 10; index++) {
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[index].type == 0x109) {
                if (!(((struct Ovl11Scan109Globals *)D_8006C838)->records[index].flags & 0x40000)) {
                    count = (s16)(count + 1);
                }
                if (count == ordinal) {
                    result = index;
                    break;
                }
            }
        }
        break;
    case 1:
        for (index = 0; index < 20; index++) {
            recordBase = (u8 *)D_8006C838 + index * 0xB8;
            if (*(u16 *)(recordBase + 0x81BC) == 0x162) {
                if (filter == 0) {
                    if (*(u16 *)(recordBase + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                } else if (filter == 1) {
                    if (*(u16 *)(recordBase + 0x826A) == 0 && *(u16 *)(recordBase + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                }
                if (count == ordinal) {
                    result = index;
                    break;
                }
            }
        }
        break;
    case 2:
        for (index = 0; index < 20; index++) {
            recordBase = (u8 *)D_8006C838 + index * 0xB8;
            if (*(u16 *)(recordBase + 0x81BC) == 0x165) {
                if (filter == 0) {
                    count = (s16)(count + 1);
                } else if (filter == 1) {
                    if (*(u16 *)(recordBase + 0x8270) >= 7 && *(u16 *)(recordBase + 0x826A) == 0 &&
                        *(u16 *)(recordBase + 0x826E) == 0) {
                        count = (s16)(count + 1);
                    }
                }
                if (count == ordinal) {
                    result = index;
                    break;
                }
            }
        }
        break;
    }
    if (ordinal == -1) {
        return count;
    }
    return result;
}
