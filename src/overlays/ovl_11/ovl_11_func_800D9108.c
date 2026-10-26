#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_map_grid_cells.h"
#include "ovl11_grid_position.h"

s32 ovl_11_func_800D5C3C(s16 arg0);

s32 ovl_11_func_800D9108(s32 *arg0) {
    s16 col;
    s16 row;
    s16 threshold;
    s32 result;
    s32 i;
    s32 j;
    Ovl11D124Entry *entry;

    result = 0;
    threshold = ovl_11_func_800D5C3C(0x36);
    if (Ovl11LookupMode1GridPosition(arg0, (u16 *)&col, (u16 *)&row) == 0) {
        row--;
        row = row < 0 ? 0 : row;
        col--;
        col = col < 0 ? 0 : col;
        for (i = (s16)row; i < (s16)row + 3 && i < 25; i++) {
            for (j = (s16)col; j < (s16)col + 3 && j < 45; j++) {
                entry = &gMainAreaGrid[i][j];
                if ((u16)entry->unk0 == 0x36 && entry->unk4 >= threshold) {
                    Ovl11RefreshCellTextureVariant(i, j, 0);
                    result = 1;
                    goto done;
                }
            }
        }
    }
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 45; j++) {
            entry = &gMainAreaGrid[i][j];
            if ((u16)entry->unk0 == 0x36 && entry->unk4 >= threshold) {
                Ovl11RefreshCellTextureVariant(i, j, 0);
                result = 1;
                goto done;
            }
        }
    }
done:
    return result;
}
