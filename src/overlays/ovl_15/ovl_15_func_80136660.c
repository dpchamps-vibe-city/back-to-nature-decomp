#include "common.h"
#include "game_types.h"
#include "psyq/libmcrd.h"

long MemCardSync(long mode, long *cmds, long *rslt);
long MemCardWriteFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes);
void ovl_15_func_80137544(char *arg0, s32 arg1);
extern u16 D_80137598;
extern s8 D_80140EC0;

s32 ovl_15_func_80136660(s16 arg0, s16 arg1, u32 *arg2) {
    s32 sp18;
    s32 sp1C;
    s32 temp_a2;
    s32 var_s2;
    s32 var_v0;

    var_s2 = 0;
    if (arg0 != 0) {
        var_v0 = 0xC;
        if (arg0 == 1) {
            var_s2 = 0x10;
            goto block_3;
        }
        return var_v0;
    }
block_3:
    ovl_15_func_80137544(&D_80140EC0, (s32) arg1);
    D_80137598 = 0;
    temp_a2 = MemCardSync(1, &sp18, &sp1C);
loop_4:
    if (sp18 != 4) {
        var_v0 = 0xC;
        if (temp_a2 != 0) {
            if (temp_a2 > 0) {
                if (temp_a2 != 1) {
                    return var_v0;
                }
                MemCardWriteFile(var_s2, &D_80140EC0, arg2, 0, 0x8000);
                D_80137598 = 0;
                return 3;
            }
            goto block_11;
        }
        return var_v0;
    }
    var_v0 = 3;
    if (temp_a2 == 0) {
        return var_v0;
    }
    if (temp_a2 > 0) {
        goto block_positive;
    }
    goto block_11;
block_11:
    if (temp_a2 == -1) {
        goto block_16;
    }
    return 0xC;
block_positive:
    if (temp_a2 != 1) {
        return 0xC;
    }
    goto block_18;
block_16:
    MemCardWriteFile(var_s2, &D_80140EC0, arg2, 0, 0x8000);
    D_80137598 = 0;
    return 3;
block_18:
    switch (sp1C) {
    case 2:
    case 5:
        if ((s16) D_80137598 >= 7) {
            return 2;
        }
        D_80137598 = (u16) D_80137598 + 1;
        goto loop_4;
    case 1:
        if ((s16) D_80137598 < 7) {
            D_80137598 = (u16) D_80137598 + 1;
            goto loop_4;
        }
        return 4;
    case 3:
        return 5;
    case 0:
        return 0;
    default:
        return 0xC;
    }
}
