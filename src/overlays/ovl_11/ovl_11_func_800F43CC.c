#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s16 pad6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
} Ov11_4360struct;

void ovl_11_func_800F4360(Ov11_4360struct *arg0, u16 arg1, u16 arg2, s32 x, s32 y, s32 z, s32 w);

void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6) {
    Ov11_4360struct *var_a3;
    char *p;
    char *p_1;
    char *p_2;
    u16 temp_a1;

    temp_a1 = arg0 & 0xFFFF;
    switch (temp_a1) {
    case 0xE8:
    case 0x122:
    case 0x123:
    case 0x124:
        p = (char *)&D_8006C838;
        var_a3 = (Ov11_4360struct *)(*(s32 *)(p + 0x8000 + 0x5DCC) + (arg6 * 0x18));
        break;
    case 0x65:
        p_1 = (char *)&D_8006C838;
        var_a3 = (Ov11_4360struct *)((u8 *)(*(Ov11_4360struct **)(p_1 + 0x8000 + 0x5DD4)) + 0x18);
        break;
    case 0x64:
        p_2 = (char *)&D_8006C838;
        var_a3 = *(Ov11_4360struct **)(p_2 + 0x8000 + 0x5DD4);
        break;
    default:
        return;
    }
    ovl_11_func_800F4360(var_a3, temp_a1, arg5, position.vx, position.vy, position.vz, position.pad);
}
