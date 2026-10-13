#include "common.h"
#include "game_types.h"

void ovl_11_func_800F5698(s16 arg0, Ovl11DispatchRecord arg1[], s32 arg2, s32 arg3, s32 arg4);
void ovl_11_func_800F4618(void);
void ovl_11_func_800F5740(void);

void ovl_11_func_800F5404(void) {
    u8 *enabled;
    char *far_base;
    Ovl11DispatchRecord *entries;
    s16 count;
    s32 flags;
    s32 i;
    s32 j;

    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[0];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[0];
    flags = ((struct Ovl11FlagRefreshState *) D_8006C838)->status;
    ovl_11_func_800F5698(0x83, entries, count, flags, 0x400);
    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[1];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[1];
    ovl_11_func_800F5698(0x71, entries, count, flags, 2);
    ovl_11_func_800F5698(0x188, entries, count,
        ((struct Ovl11FlagRefreshState *) D_8006C838)->status2, 0x40);
    flags = ((struct Ovl11FlagRefreshState *) D_8006C838)->flags;
    ovl_11_func_800F5698(0x72, entries, count, flags, 1);
    ovl_11_func_800F5698(0x73, entries, count, flags, 2);
    ovl_11_func_800F5698(0x74, entries, count, flags, 4);
    ovl_11_func_800F5698(0x75, entries, count, flags, 8);
    ovl_11_func_800F5698(0x76, entries, count, flags, 0x10);
    ovl_11_func_800F5698(0x77, entries, count, flags, 0x20);
    ovl_11_func_800F5698(0x78, entries, count, flags, 0x40);
    ovl_11_func_800F5698(0x79, entries, count, flags, 0x80);

    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[2];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[2];
    enabled = ((struct Ovl11FlagRefreshState *) D_8006C838)->enabled;
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            entries[i].flags |= 1;
        } else {
            entries[i].flags &= 0xFFFE;
        }
    }
    entries = ((struct Ovl11FlagRefreshState *) D_8006C838)->lists[3];
    count = ((struct Ovl11FlagRefreshState *) D_8006C838)->counts[3];
    enabled = ((struct Ovl11FlagRefreshState *) D_8006C838)->enabled2;
    for (j = 0; j < count; j++) {
        if (enabled[j] != 0) {
            entries[j].flags |= 1;
        } else {
            entries[j].flags &= 0xFFFE;
        }
    }
    ovl_11_func_800F4618();
    far_base = (char *) &D_8007AFF0;
    if (*(s16 *) (far_base + 0x25476) == 2) {
        ovl_11_func_800F5740();
    }
}
