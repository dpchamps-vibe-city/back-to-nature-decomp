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
#include "ovl11_record_partition_flags.h"

#include "game_types.h"
#include "ovl11_dispatch_record_flags.h"

void ovl_11_func_800F40A4(void) {
    char *base;
    Ovl11DispatchRecord **p1;
    s16 *p2;
    s32 n;

    base = (char *)&D_8006C838;
    p1 = (Ovl11DispatchRecord **)(base + 0xDD8C);
    p2 = (s16 *)(base + 0xDDD8);
    for (n = 0; n < 19; n++) {
        Ovl11ApplyDispatchRecordFlagRules(*p2, *p1);
        p1++;
        p2++;
    }
    Ovl11SetPartition6Enabled(0);
}
