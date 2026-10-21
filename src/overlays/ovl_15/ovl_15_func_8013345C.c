#include "common.h"
#include "game_types.h"

void *ovl_15_func_8013345C(Ovl15RecordPools *pools, s16 kind, s16 *index) {
    void *record = 0;
    s32 i;

    switch (kind) {
    case 0:
        for (i = 0; i < 10; i++) {
            if (pools->small[i].type == 0 && !(pools->small[i].flags & 0x02000000)) {
                record = &pools->small[i];
                *index = i;
                break;
            }
        }
        break;
    case 1:
        for (i = 0; i < 20; i++) {
            if (pools->large[i].type == 0 && !(pools->large[i].flags & 0x02000000)) {
                record = &pools->large[i];
                *index = i;
                break;
            }
        }
        break;
    }
    return record;
}
