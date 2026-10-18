#include "common.h"

void ovl_15_func_8012ED18 (void);
void ovl_15_func_8012ED6C (void);
void ovl_15_func_8012EE84 (void);
void ovl_15_func_8012EEEC (void);
void ovl_15_func_8012EF84 (void);
s32 ovl_15_func_8012F078 (void);
s32 ovl_15_func_8012F0E8 (void);
void ovl_15_func_8012F158 (void);
void ovl_15_func_8012F300 (void);
void ovl_15_func_8012F3FC (void);
void ovl_15_func_8012F4F4 (void);
void ovl_15_func_8012F6C0 (void);
void ovl_15_func_8012F7D4 (void);
s32 ovl_15_func_8012F86C (void);
void ovl_15_func_8012F8DC (void);
s32 ovl_15_func_8012F990 (void);
s32 ovl_15_func_8012FA00 (void);
s32 ovl_15_func_8012FA70 (void);
void ovl_15_func_8012FAE0 (void);
void ovl_15_func_8012FF28 (void);
s32 ovl_15_func_8012FFD8 (void);
void ovl_15_func_80130048 (void);
void ovl_15_func_801300E4 (void);
void ovl_15_func_8013010C (void);
s32 ovl_15_func_80135E9C (s16 arg0);

void ovl_15_func_8012EA18(void) {
    s16 *temp_v0_2;
    s32 temp_v0;

    switch (D_80137584) {
    case 0:
        ovl_15_func_8012ED18();
        return;
    case 1:
        ovl_15_func_8012ED6C();
        return;
    case 2:
        ovl_15_func_8012EE84();
        break;
    case 3:
        ovl_15_func_8012EEEC();
        break;
    case 4:
        ovl_15_func_8012EF84();
        return;
    case 5:
        ovl_15_func_8012F078();
        break;
    case 6:
        ovl_15_func_8012F0E8();
        break;
    case 7:
        ovl_15_func_8012F158();
        return;
    case 8:
        ovl_15_func_8012F300();
        break;
    case 9:
        ovl_15_func_8012F3FC();
        break;
    case 10:
        ovl_15_func_8012F4F4();
        return;
    case 11:
        ovl_15_func_8012F6C0();
        break;
    case 13:
        ovl_15_func_8012F7D4();
        return;
    case 14:
        ovl_15_func_8012F86C();
        break;
    case 15:
        ovl_15_func_8012F8DC();
        return;
    case 16:
        ovl_15_func_8012F990();
        break;
    case 17:
        ovl_15_func_8012FA00();
        break;
    case 18:
        ovl_15_func_8012FA70();
        break;
    case 19:
        ovl_15_func_8012FAE0();
        break;
    case 20:
        ovl_15_func_8012FF28();
        return;
    case 21:
        ovl_15_func_8012FFD8();
        break;
    case 22:
        ovl_15_func_80130048();
        break;
    case 23:
        ovl_15_func_801300E4();
        return;
    case 24:
        ovl_15_func_8013010C();
        return;
    }
    temp_v0 = ovl_15_func_80135E9C(D_8013758C != 2 ? D_8013758C : D_8013758A);
    if (temp_v0 != 3) {
        if (temp_v0 != D_80137594[D_8013758C != 2 ? D_8013758C : D_8013758A]) {
            if (temp_v0 == 5) {
                D_80137584 = 0;
            }
            if (temp_v0 == 4) {
                if (D_8013758C == 2) {
                    D_8013758C = (s16) (u16) D_8013758A;
                }
                D_80137584 = 0x15;
            }
        }
        if (D_8013758C == 2) {
            temp_v0_2 = &D_80137594[D_8013758A];
            if ((u32) ((u16) *temp_v0_2 - 6) >= 2U) {
                *temp_v0_2 = (s16) temp_v0;
            }
            D_8013758A = D_8013758A == 0;
            return;
        }
        if ((u32) ((u16) D_80137594[D_8013758C] - 6) >= 2U) {
            D_80137594[D_8013758C] = (s16) temp_v0;
        }
    } else {
        return;
    }
}
