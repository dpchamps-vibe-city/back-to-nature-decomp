#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "ovl11_item_record_allocation.h"


void ovl_11_func_8010B64C (s32 arg0);
void ovl_11_func_800D2D54 (void *arg0);

s32 ovl_11_func_800E8760(s32 arg0) {
    if ((arg0 << 0x10) == 0) {
        Ovl11AllocateItemRecord(0x15E, -1);
    } else {
        ovl_11_func_8010B64C((s32) &gStageMode2Record);
    }
    return 1;
}
