#include "common.h"
#include "psyq/stddef.h"
#include "psyq/kernel.h"
#include "psyq/libmcrd.h"
#include "psyq/stdio.h"

s32 ovl_31_func_800B83B8(void) {
    long cmds;
    long result;
    s32 i;
    s32 blocks;

    MemCardSync(0, &cmds, &result);
    MemCardGetDirentry(0, D_800B7EF0, &D_800B88B0, &D_800B8868_fileCount, 0, 15);
    blocks = 0;
    for (i = 0; i < D_800B8868_fileCount; i++) {
        blocks += (&D_800B88B0)[i].size / 8192 + (((&D_800B88B0)[i].size % 8192) ? 1 : 0);
    }
    sprintf(D_800B8B10, D_800B7EF4, D_800B8868_fileCount, blocks);
    return 2;
}
