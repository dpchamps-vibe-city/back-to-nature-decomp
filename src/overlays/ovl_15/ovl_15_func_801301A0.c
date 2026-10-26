#include "common.h"

void ovl_15_func_80130468(void);
void ovl_15_func_801304BC(void);
void ovl_15_func_801305D4(void);
void ovl_15_func_8013063C(void);
void ovl_15_func_801306D4(void);
s32 ovl_15_func_801307D8(void);
s32 ovl_15_func_80130848(void);
void ovl_15_func_801308B8(void);
s32 ovl_15_func_80130A94(void);
void ovl_15_func_80130B04(void);
s32 ovl_15_func_80130BBC(void);
void ovl_15_func_80130C2C(void);
void ovl_15_func_80130C8C(void);
void ovl_15_func_80130D3C(void);
s32 ovl_15_func_80130DDC(void);
s32 ovl_15_func_80130E4C(void);
s32 ovl_15_func_80130EBC(void);
void ovl_15_func_80130F2C(void);
void ovl_15_func_80130FC8(void);
void ovl_15_func_80130FF0(void);
s32 ovl_15_func_80135E9C(s16 arg0);

void ovl_15_func_801301A0(void) {
    s16 *temp_v0_2;
    s32 temp_v0;

    switch (gOvl15MenuState) {
    case 0:
        ovl_15_func_80130468();
        return;
    case 1:
        ovl_15_func_801304BC();
        return;
    case 2:
        ovl_15_func_801305D4();
        break;
    case 3:
        ovl_15_func_8013063C();
        break;
    case 4:
        ovl_15_func_801306D4();
        return;
    case 5:
        ovl_15_func_801307D8();
        break;
    case 6:
        ovl_15_func_80130848();
        break;
    case 7:
        ovl_15_func_801308B8();
        return;
    case 8:
        ovl_15_func_80130A94();
        break;
    case 9:
        ovl_15_func_80130B04();
        break;
    case 10:
        ovl_15_func_80130BBC();
        break;
    case 11:
        ovl_15_func_80130C2C();
        break;
    case 12:
        ovl_15_func_80130C8C();
        return;
    case 13:
        ovl_15_func_80130D3C();
        break;
    case 14:
        ovl_15_func_80130DDC();
        break;
    case 15:
        ovl_15_func_80130E4C();
        break;
    case 16:
        ovl_15_func_80130EBC();
        break;
    case 17:
        ovl_15_func_80130F2C();
        break;
    case 18:
        ovl_15_func_80130FC8();
        return;
    case 19:
        ovl_15_func_80130FF0();
        return;
    }

    temp_v0 = ovl_15_func_80135E9C(gOvl15SelectedCard != 2 ? gOvl15SelectedCard : D_8013758A);
    if (temp_v0 != 3) {
        if (temp_v0 != gOvl15CardStates[gOvl15SelectedCard != 2 ? gOvl15SelectedCard : D_8013758A]) {
            if (temp_v0 == 5) {
                gOvl15MenuState = 0;
            }
            if (temp_v0 == 4) {
                if (gOvl15SelectedCard == 2) {
                    gOvl15SelectedCard = (s16) (u16) D_8013758A;
                }
                gOvl15MenuState = 0x10;
            }
        }
        if (gOvl15SelectedCard == 2) {
            temp_v0_2 = &gOvl15CardStates[D_8013758A];
            if ((u32) ((u16) *temp_v0_2 - 6) >= 2U) {
                *temp_v0_2 = (s16) temp_v0;
            }
            D_8013758A = D_8013758A == 0;
            return;
        }
        if ((u32) ((u16) gOvl15CardStates[D_8013758A] - 6) >= 2U) {
            gOvl15CardStates[gOvl15SelectedCard] = (s16) temp_v0;
        }
    } else {
        return;
    }
}
