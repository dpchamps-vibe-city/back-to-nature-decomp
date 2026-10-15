#include "common.h"
#include "game_types.h"

s32 ovl_11_func_801032BC(Ovl11IdAdjustmentObject *arg0, s32 arg1, u16 *arg2);

s32 ovl_11_func_80102F14(Ovl11IdAdjustmentObject *arg0, u16 *arg1) {
    s16 sum;
    s32 i;
    s32 j;
    u16 id;
    u16 mask;
    u16 *entry;
    u16 key;

    i = 0;
    mask = 1;
    sum = 0;
    for (; i < 12; i++, mask <<= 1) {
        id = 0;
        if (D_801273EE_flags & mask) {
            switch (mask) {
            case 1: id = 0x72; break;
            case 2: id = 0x73; break;
            case 4: id = 0x74; break;
            case 8: id = 0x75; break;
            case 0x10: id = 0x76; break;
            case 0x20: id = 0x77; break;
            case 0x40: id = 0x78; break;
            case 0x80: id = 0x17C; break;
            case 0x100: id = 0x17D; break;
            case 0x200: id = 0x17E; break;
            case 0x400: id = 0x17F; break;
            case 0x800: id = 0x180; break;
            }
            for (j = 0; arg0->adjustments[j].id != 0; j++) {
                if (arg0->adjustments[j].id == id) {
                    sum += (s8)arg0->adjustments[j].adjustment;
                    break;
                }
            }
            if (arg0->adjustments[j].id == 0) {
                return 0;
            }
        }
    }
    i = 0;
    entry = D_8012CF00_ids;
    for (; i < 8; i++, entry++) {
        key = *entry;
        if (key != 0) {
            switch (ovl_11_func_801032BC(arg0, key, (u16 *)&sum)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; arg0->adjustments[j].id != 0; j++) {
                    if (arg0->adjustments[j].id == key) {
                        sum = (u16)sum + (s8)arg0->adjustments[j].adjustment;
                        break;
                    }
                }
                if (arg0->adjustments[j].id == 0) {
                    return 0;
                }
                break;
            }
        }
    }
    *arg1 += (u16)sum;
    return 1;
}
