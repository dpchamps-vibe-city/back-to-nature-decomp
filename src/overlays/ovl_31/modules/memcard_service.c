/*
 * memcard_service — reconstructed translation unit.
 *
 * Memory-card service helpers of the memory-card utility overlay
 * (Obj\gf_mcard.bin). Each helper first waits on MemCardSync, then performs
 * one libmcrd/libmcx operation for slot 0 (format, unformat, directory
 * listing, card probe, file rename) and returns a small status for its
 * caller, the overlay's driver ovl_31_func_800B7FCC. The driver belongs to
 * the same original file but stays outside this unit for now: the original
 * .rodata interleaves its jump table and the helpers' string literals, which
 * a pooled section reproduces only once those literals are written in C.
 *
 * The boundary is a reconstructed hypothesis; its evidence is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/kernel.h"
#include "psyq/libgte.h"
#include "psyq/libmcrd.h"
#include "psyq/stdio.h"
#include "psyq/libmcx.h"
#include "psyq/libapi.h"

s32 ovl_31_func_800B82E8(void) {
    long cmds;
    long result;
    s32 status;

    status = 0;
    MemCardSync(0, &cmds, &result);
    result = MemCardFormat(0);
    if (result != 0) {
        if (result >= 0) {
            if (result < 3) {
                status = -1;
            }
        }
    } else {
        status = 1;
    }
    return status;
}

s32 ovl_31_func_800B8348(void) {
    long cmds;
    long result;
    s32 status;

    status = 0;
    MemCardSync(0, &cmds, &result);
    result = MemCardUnformat(0);
    if (result >= -1) {
        if (result <= 0) {
            goto setneg;
        } else if (result == 1) {
            goto setpos;
        }
        goto done;
setneg:
        status = -1;
        goto done;
setpos:
        status = 1;
    }
done:
    return status;
}

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

s32 ovl_31_func_800B8490(void) {
    s32 sp10;
    s32 sp14;
    s32 ret;

    MemCardSync(0, &sp10, &sp14);
    MemCardAccept(0);
    ret = MemCardSync(0, &sp10, &sp14);

    switch (sp14) {
    case 2:
        ret = -2;
        break;
    case 4:
        McxCardType(0);
        ret = McxSync(0, &sp10, &sp14);
        switch (sp14) {
        case 0:
            ret = -3;
            break;
        case 1:
            ret = -4;
            break;
        case 3:
            ret = -5;
            break;
        case 2:
            ret = -6;
            break;
        }
        break;
    case 0:
        McxCardType(0);
        ret = McxSync(0, &sp10, &sp14);
        switch (sp14) {
        case 0:
            ret = -7;
            break;
        case 1:
            ret = -4;
            break;
        case 3:
            ret = -5;
            break;
        case 2:
            ret = -8;
            break;
        }
        break;
    case 3:
        ret = -5;
        break;
    case 1:
        ret = -4;
        break;
    }
    return (s16)ret;
}

s32 ovl_31_func_800B8600(void) {
    s32 var_s0;

    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7F2C);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F40);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, Ovl31_D_800B7F54);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F68);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7F7C);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7F90);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    sprintf(D_800B8870, D_800B7F20, 0, 0, D_800B7FA4);
    sprintf(D_800B8890, D_800B7F20, 0, 0, D_800B7FB8);
    var_s0 = 0;
    while (rename(D_800B8870, D_800B8890) != 1) {
        if (var_s0 < 3) {
            var_s0++;
        } else {
            break;
        }
    }
    return 1;
}
