#include "common.h"
#include "game_types.h"

s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

typedef struct {
    /* 0x00 */ u8 pad[0x2E];
    /* 0x2E */ u16 unk2E;
    /* 0x30 */ u16 unk30;
} Ovl11FuncD5B0Sub;

typedef struct {
    /* 0x00 */ u8 pad[0x28];
    /* 0x28 */ u16 unk28;
    /* 0x2A */ u8 pad2[0x2];
    /* 0x2C */ Ovl11FuncD5B0Sub *unk2C;
} Ovl11FuncD5B0Entry;

void ovl_11_func_800DD6F0(void) {
    u32 temp_s2;
    s32 temp_v0;
    Ovl11FuncD5B0Entry *temp_s0;
    Ovl11FuncD5B0Entry *temp_s1;
    Ovl11FuncD5B0Sub *temp_a0;
    Ovl11FuncD5B0Sub *temp_a0_2;
    Ovl11FuncD5B0Sub *temp_a1;
    Ovl11FuncD5B0Sub *temp_a2;
    Ovl11FuncD5B0Sub *temp_a3;
    Ovl11FuncD5B0Sub *temp_a4;

    if (D_80129188[0] != 0) {
        temp_s2 = (u32)D_80129190;
        switch (temp_s2) {
        case 0:
            temp_v0 = func_8001AF44(9U);
            if (temp_v0 == 1) {
                D_80129190 = temp_v0;
                return;
            }
            break;
        case 1:
            temp_s0 = (Ovl11FuncD5B0Entry *)D_80129188[0];
            temp_a0 = temp_s0->unk2C;
            temp_s1 = (Ovl11FuncD5B0Entry *)D_80129188[1];
            if ((s16)temp_a0->unk2E >= (s16)temp_a0->unk30) {
                if (func_8001AF44(9U) == temp_s2) {
                    temp_a3 = temp_s1->unk2C;
                    temp_a4 = temp_s0->unk2C;
                    temp_a3->unk2E = 0;
                    temp_a4->unk2E = 0;
                    temp_s0->unk28 |= 0x1000;
                    temp_s1->unk28 &= 0xEFFF;
                    func_8001AF70(9U, 0U);
                }
                if (func_8001AF44(0xAU) == temp_s2) {
                    D_80129190 = 2;
                    return;
                }
            } else {
                temp_a0->unk2E = temp_a0->unk2E + 1;
                return;
            }
            break;
        case 2:
            temp_s0 = (Ovl11FuncD5B0Entry *)D_80129188[1];
            temp_s0->unk2C->unk2E = temp_s0->unk2C->unk2E + 1;
            temp_a0_2 = temp_s0->unk2C;
            if ((s16)temp_a0_2->unk2E >= (s16)temp_a0_2->unk30) {
                temp_a0_2->unk2E = temp_a0_2->unk30;
                D_80129190 = 3;
                return;
            }
            break;
        case 3:
            if (func_8001AF44(0xAU) == 0) {
                temp_s0 = (Ovl11FuncD5B0Entry *)D_80129188[0];
                temp_s1 = (Ovl11FuncD5B0Entry *)D_80129188[1];
                temp_s0->unk28 &= 0xEFFF;
                temp_s1->unk28 |= 0x1000;
                temp_a1 = temp_s1->unk2C;
                temp_a2 = temp_s0->unk2C;
                temp_a1->unk2E = 0;
                temp_a2->unk2E = 0;
                D_80129190 = 0;
            }
            break;
        }
    }
}
