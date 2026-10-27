#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_item_id_flags.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_stage_mode2_record.h"


s32 ovl_11_func_800E8760(s32 arg0) {
    if ((arg0 << 0x10) == 0) {
        Ovl11AllocateItemRecord(0x15E, -1);
    } else {
        Ovl11ClearStageMode2Record((s32) &gStageMode2Record);
    }
    return 1;
}
