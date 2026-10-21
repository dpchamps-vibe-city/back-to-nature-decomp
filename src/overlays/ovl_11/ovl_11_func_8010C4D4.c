#include "common.h"

s32 ovl_11_func_8010C4D4(void) {
    s32 i;
    Ovl11ItemCell *cell;
    s32 j;
    s32 id;

    for (i = 0; i < 3; i++) {
        switch (i) {
        case 0:
            id = 0xA1;
            break;
        case 1:
            id = 0xA2;
            break;
        case 2:
            id = 0xA3;
            break;
        default:
            id = 0xA3;
            break;
        }
        cell = D_80075854_cells;
        for (j = 0; j < 99; j++) {
            if (cell->id == id) {
                cell->id = 0;
                return id;
            }
            cell++;
        }
    }
    return 0;
}
