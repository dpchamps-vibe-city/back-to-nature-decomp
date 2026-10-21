#include "common.h"
#include "game_types.h"

void ovl_11_func_800F43CC(s32 arg0, E21E4Position position, u16 selector, s32 arg3);

void ovl_11_func_8010B05C(Ovl11B05CObject *arg0) {
    Ovl11B05CObject local;

    local = *arg0;
    ovl_11_func_800F43CC(0x65, local.position, local.selector, 0);
}
