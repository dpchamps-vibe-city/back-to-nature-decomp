#include "common.h"
#include "game_flags.h"

void ovl_11_func_800DBE9C(void);

typedef s32 (*DBE30Callback)(void *);

extern DBE30Callback D_800B94AC[];
extern s32 D_800742AC;

s32 ovl_11_func_800DBE30(s32 arg0) {
    DBE30Callback *slot;
    DBE30Callback *table;

    table = D_800B94AC;
    slot = &table[arg0];
    if ((*slot)(slot) != 1) {
        SetGameFlag(2, 1);
        return 0;
    }
    SetGameFlag(2, 0);
    D_800742AC = 0;
    ovl_11_func_800DBE9C();
    return 1;
}
