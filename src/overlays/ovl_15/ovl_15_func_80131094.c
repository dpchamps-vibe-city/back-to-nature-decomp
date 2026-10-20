#include "common.h"

void ovl_15_func_8013143C (void);
void ovl_15_func_80131498 (void);
void ovl_15_func_801315B0 (void);
void ovl_15_func_80131618 (void);
void ovl_15_func_801316B0 (void);
s32 ovl_15_func_80131778 (void);
s32 ovl_15_func_801317E8 (void);
void ovl_15_func_80131858 (void);
s32 ovl_15_func_80131A34 (void);
void ovl_15_func_80131AA4 (void);
s32 ovl_15_func_80131B5C (void);
void ovl_15_func_80131BCC (void);
void ovl_15_func_80131C2C (void);
void ovl_15_func_80131CDC (void);
s32 ovl_15_func_80131D88 (void);
s32 ovl_15_func_80131DF8 (void);
s32 ovl_15_func_80131E68 (void);
void ovl_15_func_80131ED8 (void);
s32 ovl_15_func_80131F5C (void);
void ovl_15_func_80131FD0 (void);
void ovl_15_func_8013203C (void);
void ovl_15_func_801320A8 (void);
void ovl_15_func_8013237C (void);
void ovl_15_func_80132408 (void);
void ovl_15_func_80132494 (void);
void ovl_15_func_80132520 (void);
void ovl_15_func_801325AC (void);
void ovl_15_func_80132638 (void);
void ovl_15_func_801326E8 (void);
void ovl_15_func_8013280C (void);
s32 ovl_15_func_801328C4 (void);
void ovl_15_func_80132934 (void);
void ovl_15_func_801329D0 (void);
void ovl_15_func_801329F8 (void);
s32 ovl_15_func_80135E9C (s16 arg0);

void ovl_15_func_80131094(void) {
    s16 *temp_v0_2;
    s32 temp_v0;

    switch (D_80137584) {
    case 0:
        ovl_15_func_8013143C();
        return;
    case 1:
        ovl_15_func_80131498();
        return;
    case 2:
        ovl_15_func_801315B0();
        break;
    case 3:
        ovl_15_func_80131618();
        break;
    case 4:
        ovl_15_func_801316B0();
        return;
    case 5:
        ovl_15_func_80131778();
        break;
    case 6:
        ovl_15_func_801317E8();
        break;
    case 7:
        ovl_15_func_80131858();
        return;
    case 8:
        ovl_15_func_80131A34();
        break;
    case 9:
        ovl_15_func_80131AA4();
        break;
    case 10:
        ovl_15_func_80131B5C();
        break;
    case 11:
        ovl_15_func_80131BCC();
        break;
    case 12:
        ovl_15_func_80131C2C();
        return;
    case 13:
        ovl_15_func_80131CDC();
        break;
    case 14:
        ovl_15_func_80131D88();
        break;
    case 15:
        ovl_15_func_80131DF8();
        break;
    case 30:
        ovl_15_func_80131E68();
        break;
    case 16:
        ovl_15_func_80131ED8();
        break;
    case 17:
        ovl_15_func_80131F5C();
        break;
    case 18:
        ovl_15_func_80131FD0();
        break;
    case 19:
        ovl_15_func_8013203C();
        break;
    case 20:
        ovl_15_func_801320A8();
        break;
    case 21:
        ovl_15_func_8013237C();
        break;
    case 22:
        ovl_15_func_80132408();
        break;
    case 23:
        ovl_15_func_80132494();
        break;
    case 24:
        ovl_15_func_80132520();
        break;
    case 25:
        ovl_15_func_801325AC();
        break;
    case 26:
        ovl_15_func_80132638();
        break;
    case 27:
        ovl_15_func_801326E8();
        break;
    case 29:
        ovl_15_func_8013280C();
        return;
    case 28:
        ovl_15_func_801328C4();
        break;
    case 31:
        ovl_15_func_80132934();
        break;
    case 32:
        ovl_15_func_801329D0();
        return;
    case 33:
        ovl_15_func_801329F8();
        return;
    default:
        break;
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
                D_80137584 = 0x1E;
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
        if ((u32) ((u16) D_80137594[D_8013758A] - 6) >= 2U) {
            D_80137594[D_8013758C] = (s16) temp_v0;
        }
    } else {
        return;
    }
}
