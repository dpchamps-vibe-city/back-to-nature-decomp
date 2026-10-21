#include "common.h"

void ovl_11_func_800BD3C4(s32 arg0) {
    s32 *range;
    s32 *table;
    s32 size;

    range = &D_801227B0[arg0];
    func_80014BCC(0, range[0], range[1] - range[0], 0, D_8005E3B0 + 0x4290);
    ((Ovl11LoadedItemView *)&D_8007AFF0)->loadedItem = arg0;
    table = D_80124FCC_tables[arg0];
    size = *table;
    size = table[size + 1];
    memcpy(D_8008F7F8_words, (void *)(D_8005E3B0 + 0x4290), size);
    func_8001719C((u8 *)D_8005E3B0 + 0x4290 + size);
}
