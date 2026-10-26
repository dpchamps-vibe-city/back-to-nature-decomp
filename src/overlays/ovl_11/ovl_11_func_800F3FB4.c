#include "common.h"
#include "ovl11_projected_record_sprites.h"
#include "ovl11_record_pool_refresh.h"

void ovl_11_func_800F4A58(void);

void ovl_11_func_800F3FB4(void) {
    if (!(D_8006C848 & 0x4000)) {
        Ovl11RefreshDispatchRecordPools();
        Ovl11DrawSceneRecordSprites();
        ovl_11_func_800F4A58();
    }
}
