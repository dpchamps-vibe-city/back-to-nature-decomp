#include "common.h"
#include "random.h"

u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
void ovl_11_func_800CF748(void);
void ovl_11_func_800CF848(void);
s32 ovl_11_func_800D2E20(void);

void ovl_11_func_800CF428(void) {
    char *base;
    char *initial_base;
    char *final_base;
    char *far_base;
    char *special_base;
    char *last_base;
    char *object;
    s16 *entry;
    s32 i;
    s32 hour;
    s16 id;
    s16 mask;
    s32 unused[2];

    ovl_11_func_800CF748();
    ovl_11_func_800CF848();
    initial_base = (char *)D_8006C838;
    if (*(s16 *)(initial_base + 0xE4C8) != 0) {
        return;
    }
    mask = ovl_11_func_800D2E20();
    entry = (s16 *)gOvl11ItemPlacementRules;
    i = 0;
    for (; i < 17; i++) {
        if (!((D_80128D66 >> i) & 1)) {
            far_base = (char *)&D_8007AFF0;
            if (*(s16 *)(far_base + 0x25476) == entry[1] &&
                (mask & (u16)entry[2])) {
                id = entry[0];
                if (id != 0x163 && id != 0x166 && Rand(10) != 0) {
                    entry += 12;
                    continue;
                }
                object = (char *)ovl_11_func_800CE744(entry[0], -1);
                if (object != 0) {
                    *(s32 *)(object + 0x38) = *(s32 *)(entry + 4);
                    *(s32 *)(object + 0x3C) = *(s32 *)(entry + 6);
                    *(s32 *)(object + 0x40) = *(s32 *)((char *)entry + 0x10);
                    if (Rand(2) != 0) {
                        *(s16 *)(object + 0x22) = 0;
                    } else {
                        *(s16 *)(object + 0x22) = 3;
                    }
                    id = entry[0];
                    switch (id) {
                    case 0x163:
                    case 0x166:
                        break;
                    default:
                        D_80128D66 = D_80128D66 | (1 << i);
                        break;
                    }
                }
            }
        }
        entry += 12;
    }
    special_base = (char *)&D_8007AFF0;
    if (*(s16 *)(special_base + 0x25476) != 1) {
        return;
    }
    if (*(s16 *)(*(char **)(special_base + 0x2538C) + 2) == 2) {
        object = (char *)D_8006C838;
        hour = *(s16 *)(object + 0x44C0);
        if (hour < 6) {
            goto special_done;
        }
        if (hour >= 9) {
            goto special_done;
        }
        if (Rand(10) == 0 && *(s16 *)(object + 0x44BA) != 3 &&
            !((u16)D_80128D66 >> 15)) {
            i = 0;
            base = object;
            for (; i < 3; i++) {
                object = (char *)ovl_11_func_800CE744(0x154, -1);
                if (object != 0) {
                    *(s32 *)(object + 0x38) = D_801238F0[i][0];
                    *(s32 *)(object + 0x3C) = D_801238F0[i][1];
                    *(s32 *)(object + 0x40) = D_801238F0[i][2];
                    *(s16 *)(object + 0x22) = i;
                }
                *(s32 *)(base + 0x44F8) |= 0x8000;
            }
        }
    }
special_done:
    last_base = (char *)&D_8007AFF0;
    if (*(s16 *)(last_base + 0x25476) == 1 &&
        *(s16 *)(*(char **)(last_base + 0x2538C) + 2) != 12) {
        final_base = (char *)D_8006C838;
        hour = *(s16 *)(final_base + 0x44C0);
        if (hour < 18) {
            return;
        }
        if (hour >= 23) {
            return;
        }
        if ((*(s32 *)(final_base + 0x44F8) & 0x1800000) == 0x800000) {
            ovl_11_func_800CE744(0x15B, -1);
        }
    }
}
