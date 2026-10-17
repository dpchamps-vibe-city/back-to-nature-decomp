#include "common.h"
#include "game_types.h"

void ovl_23_func_800BABD8(void);
void ovl_23_func_800BA73C(Ovl23BA73CArg *arg0);
void ovl_23_func_800BAA60(Ovl23BAA60Arg *arg0);
void ovl_23_func_800B954C();

void ovl_23_func_800B9310(void) {
    Ovl23BA73CArg *actor;
    Ovl23BAA60Arg *record;
    Ovl23BA73CArg *base;
    s32 i;

    ovl_23_func_800BABD8();
    i = 0;
    base = (Ovl23BA73CArg *)D_800BF8A0;
    record = (Ovl23BAA60Arg *)(base + 6);
    actor = base;
    do {
        ovl_23_func_800BA73C(actor);
        ovl_23_func_800BAA60(record);
        record++;
        i++;
        actor++;
    } while (i < 6);
    ovl_23_func_800B954C();
}
