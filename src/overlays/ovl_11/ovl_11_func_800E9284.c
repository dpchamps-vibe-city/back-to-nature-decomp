#include "common.h"

void ovl_11_func_800C401C(void);

s32 ovl_11_func_800E9284(s16 arg0, s16 arg1) {
    s32 temp_s0;
    s32 index = arg1;

    if (index >= 0x32) {
        u8 *base = (u8 *) D_8006C838;
        u8 *record;

        index -= 0x32;
        record = base + index * 0xF8;

        ((struct struct_8006C838_800E65AC_b *) record)->field_DE22 = arg0;
    } else {
        if (((Ovl11CBB9CGlobals *) D_8006C838)->state != arg0) {
            temp_s0 = ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C;
            ((Ovl11CBB9CGlobals *) D_8006C838)->state = arg0;
            ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C = temp_s0 & 0xF7FFFFFF;
            ovl_11_func_800C401C();
            ((struct struct_8006C838_800E4608 *) D_8006C838)->field_0C = temp_s0;
            return 1;
        }
        return 1;
    }
    return 1;
}
