#include "common.h"
#include "game_types.h"

s32 func_8001FABC(s16 arg0);
void func_8001FAE8(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void ovl_19_func_800BAC5C(void);
void ovl_19_func_800BAC7C(void);
void ovl_19_func_800B942C(void);
s32 ovl_19_func_800BA25C(void);
void ovl_19_func_800BAC50(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2);
void ovl_19_func_800B998C(void);
void ovl_19_func_800B9454(void);
void ovl_19_func_800B95D4(void);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);
void ovl_19_func_800BA770(void);

void ovl_19_func_800B89C8(void) {
    s32 selection;
    s16 quotient;
    Ovl19Func800BAC40Arg *other;

    if (((s32 *)D_8005E3A8)[2] & 0x800) {
        *(s32 *)D_800BF4C0 = *(s32 *)D_800BF4C0 != 1;
        if (*(s32 *)D_800BF4C0 == 1) {
            ovl_19_func_800BAC5C();
            D_800BF4C0[0x101] = func_8001FABC(0x36);
        } else {
            ovl_19_func_800BAC7C();
        }
    }
    if (*(s32 *)D_800BF4C0 == 1) {
        ovl_19_func_800B942C();
    } else {
        D_800BF4C0[5]++;
        if (D_800BF4C0[5] == 10000) {
            D_800BF4C0[5] = 0;
        }
        selection = ovl_19_func_800BA25C();
        if (selection != -1) {
            if (D_800BD070 == -1) {
                D_800BD070 = selection;
                func_8002261C(4, 0x15);
                D_800BF4C0[0x102] = func_8001FABC(0x43);
                ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50 + selection * 0x20], 3, 0);
                other = (Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50];
                if (selection == 0) {
                    other = (Ovl19Func800BAC40Arg *)&D_800BF4C0[0x70];
                }
                ovl_19_func_800BAC50(other, 0, 0);
                ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x90], 4, 0);
            }
            if (GetVal8005E5B4() == 2) {
                func_8001FAE8(D_800BF4C0[0x102]);
                D_800BF4C0[2] = 6;
                D_800BF4C0[6] = D_800BD070;
                D_800BF4C0[0x102] = func_8001FABC(0x43);
            } else {
                ovl_19_func_800B998C();
            }
        } else {
            D_800BD070 = -1;
            if (D_800BF4C0[0x51] == 1) {
                if (((s32 *)D_8005E3A8)[2] & 0x40) {
                    ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 2, 30);
                }
            } else {
                D_800BF4C0[0x52]--;
                if (D_800BF4C0[0x52] <= 0) {
                    ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 1, 0);
                }
            }
            ovl_19_func_800B9454();
            ovl_19_func_800B95D4();
            ovl_19_func_800B998C();
            if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)&D_800BF4C0[8]) == 1) {
                quotient = D_800BF4C0[5] / 30;
                if (D_800BF4C0[5] == quotient * 30) {
                    D_800BF4C0[0x101] = func_8001FABC(0x3A);
                }
            }
        }
        ovl_19_func_800BA770();
    }
}
