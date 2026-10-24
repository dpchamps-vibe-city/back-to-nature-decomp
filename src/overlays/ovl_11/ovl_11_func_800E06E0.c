#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800E109C(UnkStruct800E109C *arg0);
s32 ovl_11_func_800D0D7C(u8 *arg0);
void ovl_11_func_80107DD0(s16 *arg0);
/* Overlay import of Rand at 0x80012A34; type from src/Rand.c. */
u32 Rand(s32 arg0);

s32 ovl_11_func_800E06E0(Ovl11Deb00Object *arg0) {
    s32 i;
    s32 temp;
    s32 out;
    if (ovl_11_func_800E109C((UnkStruct800E109C *)arg0) == 0) {
        return -1;
    }
    if (arg0->field_34 & 0x400) {
        return -1;
    }
    ovl_11_func_800D0D7C((u8 *)arg0);
    if (arg0->field_30 == 5 && !(arg0->field_34 & 1)) {
        for (i = 0; i < 22; i++) {
            if (((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] != 0) {
                arg0->field_34 |= 1;
                ((Ovl11Deb00State *)D_8006C838)->field_4AC0[i] = 0;
                break;
            }
        }
    }
    if (arg0->field_34 & 2) {
        arg0->field_16 += 3;
    }
    if (arg0->field_34 & 0x10) {
        temp = arg0->field_B4;
        temp++;
        arg0->field_B4 = temp;
        if ((u16)temp >= 8) {
            out = 7;
        } else {
            out = temp;
        }
        arg0->field_B4 = out;
    }
    if (arg0->field_34 & 1) {
        arg0->field_34 |= 0x10;
        arg0->field_16++;
    } else {
        arg0->field_34 &= ~0x10;
        arg0->field_16 -= 10;
    }
    if (arg0->field_AC >= 5) {
        arg0->field_16++;
    }
    temp = arg0->field_16;
    if ((s16)temp < 0) {
        temp = 0;
    }
    arg0->field_16 = temp;
    out = temp;
    if ((s16)out >= 256) {
        out = 255;
    }
    arg0->field_16 = out;
    if (!(arg0->field_34 & 0x100)) {
        temp = arg0->field_AE;
        if (temp != 0) {
            temp++;
            arg0->field_AE = temp;
            if ((u16)temp >= 32) {
                temp = 31;
            }
            arg0->field_AE = temp;
            if (arg0->field_AE >= 3 && (s32)Rand(100) <= arg0->field_AE * 10) {
                arg0->field_34 |= 0x100;
            }
        } else if ((s16)arg0->field_16 <= 0 && Rand(100) < 11) {
            arg0->field_AE++;
        }
    }
    if (arg0->field_B2 != 0) {
        arg0->field_B2++;
        if (arg0->field_B2 >= 21) {
            arg0->field_B2 = 0;
            arg0->field_34 |= 0x200;
        }
    }
    if (!(arg0->field_34 & 0x100)) {
        if (arg0->field_1A == 3) {
            if (Rand(100) < 26) {
                arg0->field_34 |= 0x80100;
            }
        } else if (arg0->field_1A == 4) {
            if (Rand(100) < 76) {
                arg0->field_34 |= 0x80100;
            }
        }
        if (arg0->field_1A >= 5) {
            arg0->field_34 |= 0x80100;
        }
    }
    temp = arg0->field_B0;
    temp++;
    arg0->field_B0 = temp;
    out = temp;
    if ((u16)out >= 32) {
        out = 31;
    }
    arg0->field_B0 = out;
    arg0->field_8C = 0;
    if (arg0->field_1E == ((Ovl11Deb00State *)D_8006C838)->field_44BC &&
        arg0->field_1C == ((Ovl11Deb00State *)D_8006C838)->field_44BA) {
        arg0->field_1A++;
    }
    arg0->field_2C = Rand(300);
    if (!(arg0->field_34 & 1)) {
        ((Ovl11Deb00State *)D_8006C838)->field_44F8 |= 0x8000000;
    }
    arg0->field_AC = 0;
    arg0->field_34 &= ~1;
    arg0->field_34 &= ~2;
    arg0->field_34 &= ~4;
    arg0->field_34 &= ~8;
    arg0->field_34 &= ~0x2000;
    ovl_11_func_80107DD0(arg0->field_A8);
    return 0;
}
