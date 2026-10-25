#include "common.h"
#include "game_types.h"

void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226B0(void);
void func_80022738(void);
s32 func_8001FABC(s16 arg0);
void ovl_11_func_80100898(u8 arg0);
s32 ovl_11_func_80100A8C(s32 arg0);
void ovl_11_func_80103714(void);
void ovl_11_func_80100194(void);
void ovl_11_func_80100644(void);
void ovl_11_func_801006F8(void);
void ovl_11_func_801007AC(void);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_8010021C(void) {
    s8 column;

    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x3B8);
    ovl_11_func_80100898(Ovl11_D_801273E8);
    if (func_800226B0()) {
        switch ((u32)((GfxObj *)D_8005E3A8)->field_8 & 0x870) {
        case 0x800:
            func_80022738();
            Ovl11_D_801273DC = ovl_11_func_80103714;
            return;
        case 0x20:
            func_80022738();
            func_8001FABC(1);
            Ovl11_D_801273DC = ovl_11_func_80100194;
            return;
        case 0x10:
            if (((Ovl11SelectionRecords *)&D_8006C838)->records[Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7][0] != 0) {
                func_80022738();
                func_8001FABC(0);
                Ovl11_D_801273DC = ovl_11_func_80100644;
                return;
            }
            func_8001FABC(2);
            break;
        case 0x40:
            if (((Ovl11SelectionRecords *)&D_8006C838)->records[Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7][0] == 0) {
                func_8001FABC(2);
            } else if (ovl_11_func_80100A8C((u8)(Ovl11_D_801273E8 * 20 + Ovl11_D_801273E7)) == 0) {
                func_80022738();
                func_8001FABC(2);
                Ovl11_D_801273DC = ovl_11_func_801006F8;
                return;
            } else {
                func_80022738();
                func_8001FABC(0);
                Ovl11_D_801273DC = ovl_11_func_801007AC;
                return;
            }
            break;
        }
        switch (*(u32 *)D_8005E3A8 & 0xF000) {
        case 0x1000:
            func_8001FABC(5);
            Ovl11_D_801273E7--;
            if (Ovl11_D_801273E7 == -1 || Ovl11_D_801273E7 == 9) {
                Ovl11_D_801273E7 += 10;
            }
            break;
        case 0x4000:
            func_8001FABC(5);
            Ovl11_D_801273E7++;
            if (Ovl11_D_801273E7 == 10 || Ovl11_D_801273E7 == 20) {
                Ovl11_D_801273E7 -= 10;
            }
            break;
        case 0x8000:
            func_8001FABC(5);
            Ovl11_D_801273E7 -= 10;
            if (Ovl11_D_801273E7 < 0) {
                Ovl11_D_801273E7 += 20;
                Ovl11_D_801273E8--;
            }
            break;
        case 0x2000:
            func_8001FABC(5);
            Ovl11_D_801273E7 += 10;
            if (Ovl11_D_801273E7 >= 20) {
                Ovl11_D_801273E7 -= 20;
                Ovl11_D_801273E8++;
            }
            break;
        }
        if (Ovl11_D_801273E8 < 0) {
            Ovl11_D_801273E8 = 3;
        } else if (Ovl11_D_801273E8 >= 4) {
            Ovl11_D_801273E8 = 0;
        }
    }
    column = Ovl11_D_801273E7 / 10;
    func_800248B0(gActiveRenderContext->field_D8 + 0x54,
                 column * 144 + 23, (Ovl11_D_801273E7 - column * 10) * 14 + 24);
    D_801273EC = Ovl11_D_801273E7 + Ovl11_D_801273E8 * 20;
}
