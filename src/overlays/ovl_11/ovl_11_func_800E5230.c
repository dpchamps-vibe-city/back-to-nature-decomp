#include "common.h"
#include "game_types.h"
#include "ovl11_spawn_record_access.h"

void ovl_11_func_800E5230(void) {
    s32 *p;
    s32 *base;
    s32 *base2;
    s32 i;

    i = 9;
    p = (s32 *)D_80129230;
    p = (s32 *)((char *)p + 0x1C4);
    while (i >= 0) {
        *p = 0;
        p = (s32 *)((char *)p - 0x30);
        i--;
    }
    base = (s32 *)&D_8006C838;
    base2 = base + (0x8000 >> 2);
    Ovl11ClearSpawnRecords((Ovl11DispatchRecord *)base2[0x5DD0 >> 2], *(s16 *)((char *)base2 + 0x5DFA));
    *(s16 *)((char *)base + 0x4476) = 1;
}
