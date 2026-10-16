#include "common.h"
#include "game_types.h"

s32 ovl_23_func_800BA30C(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_23_func_800BA368(s32 arg0, s32 arg1);
s32 ovl_23_func_800BA278(s32 arg0);

s32 ovl_23_func_800B9E04(Ovl23Func800BA1E0Arg *arg0, s16 arg1, s16 arg2) {
    s16 y;
    s32 i;

    switch (arg1) {
    case 3:
    case 5:
        y = arg0->unk18 / 4096 - arg2 * 1000 / 1414;
        break;
    case 4:
        y = arg0->unk18 / 4096 - arg2;
        break;
    case 1:
    case 7:
        y = arg0->unk18 / 4096 + arg2 * 1000 / 1414;
        break;
    case 0:
        y = arg2 + arg0->unk18 / 4096;
        break;
    default:
        y = arg0->unk18 / 4096;
        break;
    }
    if (y < ((s16 *)D_800BBA4C)[arg0->unk0 * 2]) {
        return 0;
    }
    if (((s16 *)D_800BBA4C)[arg0->unk0 * 2 + 1] < y) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        if (i != arg0->unk0) {
            if (arg2 >= ovl_23_func_800BA30C(arg0->unk14 / 4096,
                    arg0->unk18 / 4096, Ovl23_D_800BF87C_movement.records[i].unkC / 4096,
                    ((s16 *)&D_800BBA40)[i])) {
                if (arg1 == ovl_23_func_800BA278(ovl_23_func_800BA368(
                        arg0->unk14 / 4096 - Ovl23_D_800BF87C_movement.records[i].unkC / 4096,
                        arg0->unk18 / 4096 - ((s16 *)&D_800BBA40)[i]))) {
                    return 0;
                }
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (i != arg0->unk0) {
            if (arg2 >= ovl_23_func_800BA30C(arg0->unk14 / 4096,
                    arg0->unk18 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                    Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096)) {
                if (arg1 == ovl_23_func_800BA278(ovl_23_func_800BA368(
                        arg0->unk14 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk38 / 4096,
                        arg0->unk18 / 4096 - Ovl23_D_800BF87C_movement.actors[i].unk3C / 4096))) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
