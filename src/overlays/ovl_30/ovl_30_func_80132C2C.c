#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_30_func_80132C2C(s32 arg0) {
    u32 i;
    u32 index;
    struct Ovl30NValues *data;

    if (*(s32 *)gPad0Input & 0x2000) {
        D_80134AB0 += 10;
        if (D_80134AB0 >= 0x80) {
            D_80134AB0 = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x8000) {
        D_80134AB0 -= 10;
        if (D_80134AB0 <= 0) {
            D_80134AB0 = 0x7F;
        }
    }
    if (*(s32 *)gPad0Input & 0x4000) {
        D_80134AB0++;
        if (D_80134AB0 >= 0x80) {
            D_80134AB0 = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x1000) {
        if (D_80134AB0 <= 0) {
            D_80134AB0 = 0x7F;
        } else {
            D_80134AB0--;
        }
    }
    if (*(s32 *)gPad0Input & 0x20) {
        ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0]++;
        if (((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] >= 0xFF) {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x40) {
        if (((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] == 0) {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] = 0xFF;
        } else {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0]--;
        }
    }
    if (*(s32 *)gPad0Input & 0x10) {
        ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] += 10;
        if (((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] >= 0xFF) {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] = 0;
        }
    }
    if (*(s32 *)gPad0Input & 0x80) {
        if (((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] == 0) {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] = 0xFF;
        } else {
            ((struct Ovl30NValues *)D_8006C838)->values[D_80134AB0] -= 10;
        }
    }
    data = (struct Ovl30NValues *)D_8006C838;
    FntPrint(D_8012EED0, D_80134AB0, data->values[D_80134AB0]);
    i = 0;
    do {
        index = D_80134AB0 + i;
        if (index < 0x80U) {
            FntPrint(D_8012EF14, (s32)index, data->values[index]);
        }
        i++;
    } while (i < 10U);
    return arg0;
}
