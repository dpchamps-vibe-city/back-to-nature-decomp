#include "common.h"
#include "game_types.h"

s32 ovl_11_func_801031E4(u16 arg0);

s32 ovl_11_func_80102E1C(Ovl11IdAdjustmentObject *arg0) {
    s32 i;
    s32 j;
    u16 id;

    for (i = 0; i < 8; i++) {
        id = arg0->ids[i];
        if (id != 0) {
            switch (ovl_11_func_801031E4(id)) {
            case 1:
                break;
            case 2:
                return 0;
            case 0:
            default:
                for (j = 0; j < 8; j++) {
                    if (id == D_8012CF00_ids[j]) {
                        break;
                    }
                }
                if (j == 8) {
                    return 0;
                }
                D_8012CF00_ids[j] = 0;
                break;
            }
        }
    }
    return 1;
}
