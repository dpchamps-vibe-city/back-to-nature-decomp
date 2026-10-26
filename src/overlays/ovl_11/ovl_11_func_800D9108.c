#include "common.h"

s32 ovl_11_func_800D5C3C(s16 arg0);
s32 ovl_11_func_800D7EF8(s32 *arg0, u16 *arg1, u16 *arg2);
void ovl_11_func_800DACD4(s16 arg0, s16 arg1, s32 arg2);

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
    if (ovl_11_func_800D7EF8(arg0, (u16 *)&col, (u16 *)&row) == 0) {
        row--;
        row = row < 0 ? 0 : row;
        col--;
        col = col < 0 ? 0 : col;
        for (i = (s16)row; i < (s16)row + 3 && i < 25; i++) {
            for (j = (s16)col; j < (s16)col + 3 && j < 45; j++) {
                entry = &gMainAreaGrid[i][j];
                if ((u16)entry->unk0 == 0x36 && entry->unk4 >= threshold) {
                    ovl_11_func_800DACD4(i, j, 0);
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
                ovl_11_func_800DACD4(i, j, 0);
                result = 1;
                goto done;
            }
        }
    }
done:
    return result;
}
