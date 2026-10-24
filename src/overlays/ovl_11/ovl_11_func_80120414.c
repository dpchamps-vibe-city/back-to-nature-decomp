#include "common.h"

u32 Rand(s32 arg0);
s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

s16 ovl_11_func_80120414(s32 *arg0) {
    s16 *table;
    s32 count;
    s32 total;
    s32 roll;
    s32 i;

    table = D_80128748[((struct Ovl11WeightedSelectionState *)D_8006C838)->mode];
    switch (arg0[2]) {
    case 1: table = D_801286D0; break;
    case 2: table = D_80128708; break;
    case 3: table = D_801286F8; break;
    case 4: table = D_8012872C; break;
    }
    count = *table++;
    total = 0;
    roll = Rand(100);
    for (i = 0; i < count; i++, table += 2) {
        total += table[0];
        if (roll < total) {
            switch (table[1]) {
            case 0xEA:
                if (func_8001AF44(0x5A) == 1) break;
                func_8001AF70(0x5A, 1);
                return table[1];
            case 0x67:
                if (func_8001AF44(0x53) == 1) break;
                func_8001AF70(0x53, 1);
                return table[1];
            case 0xED:
                if (func_8001AF44(0x54) != 1 &&
                    func_8001AF44(0x55) == 1 &&
                    func_8001AF44(0x56) == 1 &&
                    func_8001AF44(0x57) == 1 &&
                    func_8001AF44(0x58) == 1 &&
                    func_8001AF44(0x59) == 1) {
                    func_8001AF70(0x54, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 4;
                    return table[1];
                }
                break;
            case 0xF0:
                if (func_8001AF44(0x57) != 1 && func_8001AF44(0x5E) == 1) {
                    func_8001AF70(0x57, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x20;
                    return table[1];
                }
                break;
            case 0xF1:
                if (func_8001AF44(0x58) != 1 &&
                    (((struct Ovl11WeightedSelectionState *)D_8006C838)->time < 0x1E0 ||
                     ((struct Ovl11WeightedSelectionState *)D_8006C838)->time > 0x528)) {
                    func_8001AF70(0x58, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x40;
                    return table[1];
                }
                break;
            case 0xF2:
                if (func_8001AF44(0x59) != 1 && func_8001AF44(0x5F) == 1) {
                    func_8001AF70(0x59, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x80;
                    return table[1];
                }
                break;
            case 0xEF:
                if (func_8001AF44(0x56) == 0) {
                    func_8001AF70(0x56, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 0x10;
                    return table[1];
                }
                break;
            case 0xEE:
                if (func_8001AF44(0x55) != 1 &&
                    func_8001AF44(0x5B) == 1 &&
                    func_8001AF44(0x5C) == 1 &&
                    func_8001AF44(0x5D) == 1) {
                    func_8001AF70(0x55, 1);
                    ((struct Ovl11WeightedSelectionState *)D_8006C838)->flags |= 8;
                    return table[1];
                }
                break;
            default:
                if (table[1] >= 0xA1 && table[1] <= 0xA3) {
                    D_8012DB8C = 0;
                } else if (D_8012DB8C < 11) {
                    D_8012DB8C++;
                } else {
                    D_8012DB8C = 0;
                    return D_80128758[arg0[2]];
                }
                return table[1];
            }
        }
    }
    return 0;
}
