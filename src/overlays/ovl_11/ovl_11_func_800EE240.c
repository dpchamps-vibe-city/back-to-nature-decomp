#include "common.h"

/* Overlay import of u32 Rand(s32), at 0x80012A34. */
u32 Rand(s32 arg0);
s32 func_8001FABC(s16 arg0);
void func_800151B4(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6, u8 arg7);

s32 ovl_11_func_800EE240(s16 arg0, s16 arg1) {
    struct Ovl11ColorTriple colors = D_800B9E38;
    s16 i;
    s32 next;
    s32 index;
    s32 index2;
    s32 blue;
    s32 green;
    s32 *packet;
    s32 *base;
    s16 noise;
    s32 slot;

    if (arg0 == 0) {
        D_80129560[arg1] += 5;
        switch (D_80129560[arg1]) {
        case 0:
            func_8001FABC(0x46);
            break;
        case 300:
            i = Rand(3);
            colors.values[i] = 255;
            i = Rand(3);
            colors.values[i] = 255;
            base = D_80129560;
            for (i = 0; i < 3; i++) {
                slot = arg1 + i + 1;
                noise = Rand(32);
                base[slot] = colors.values[i];
                base[slot] -= noise;
            }
            break;
        case 330:
            func_8001FABC(0x47);
            break;
        default:
            if (D_80129560[arg1] >= 360) {
                for (i = 0; i < 3; i++) {
                    colors.values[i] = 160;
                }
                next = arg1 + 1;
                D_80129560[next] = 0;
                arg0 = 1;
            }
            break;
        }
        next = arg1 + 1;
        if (D_80129560[next] != 0) {
            packet = (s32 *)(D_8005E3C0->field_D8 + 0x14);
            index = arg1 + 3;
            blue = *(u8 *)&D_80129560[index];
            index2 = arg1 + 2;
            green = *(u16 *)&D_80129560[index2];
            func_800151B4(packet, 0, 0, 320, 240,
                blue | (green << 8) | (*(u8 *)&D_80129560[next] << 16), 1, 1);
            for (i = 1; i < 4; i++) {
                D_80129560[arg1 + i] -= 15;
                if (D_80129560[arg1 + i] < 0) {
                    D_80129560[arg1 + i] = 0;
                }
            }
        }
    }
    if (arg0 != 0) {
        D_80129560[arg1] = -((s16)Rand(80) * 5) - 200;
    }
    return 1;
}
