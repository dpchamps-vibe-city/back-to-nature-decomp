#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"
typedef struct {
    s16 unk0;
} ReconA0View;

s32 ovl_11_func_801108F8(ReconA0View *arg0) {
    s32 callRet2;
    callRet2 = Ovl11ItemHasFlag40(arg0->unk0);
    if (callRet2 == 0) {
        return 0;
    } else {
        return ((u32)0) < ((u32)(arg0->unk0 ^ 0x36));
    }
}
