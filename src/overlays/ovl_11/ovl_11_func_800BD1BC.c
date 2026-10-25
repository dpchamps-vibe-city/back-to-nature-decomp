#include "common.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void ovl_11_func_800BD1BC(s32 arg0) {
    s32 *p;

    p = &D_801227F8[arg0];
    CdReadDiscSectorsBlocking(0, p[0], p[1] - p[0], 0, gGraphicsHeapBase + 0x4290);
    memcpy(&D_8007F7F8, (void *)(gGraphicsHeapBase + 0x4290), p[1] - p[0]);
}
