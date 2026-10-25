#include "common.h"
#include "screen_fade.h"

s32 func_8001FABC(s16);
void func_8001FE34(s32);
s32 func_8001FE6C(void);
s32 GetVal8005E5B8(void);
s32 func_800225C4(void);
s32 func_8002261C(s32, s32);
s32 GetVal8005E5B4(void);
void func_8002301C(void);
void func_80023030(s32);
s32 func_80023060(void);
void func_80023100(void);
void func_80023130(u16 *);
void func_80023170(u16 *);
void func_800231AC(u16 *);
s32 func_800231E8(void);
void ovl_27_func_800B7F7C(void);
void ovl_27_func_800B9124(void);
void ovl_27_func_800B9224(void);
void ovl_27_func_800B928C(void);
void ovl_27_func_800B92E4(void);
void ovl_27_func_800BA130(s16);

void ovl_27_func_800B845C(void) {
    s32 *base;

    func_800225C4();
    D_800C4A1C = (D_800C4A1C + 1) % 108000;
    switch (D_800C4A24) {
    case 0:
        ((void (*)(void))D_800C4A14)();
        BeginScreenFadeDecrease(10, 0, 2);
        D_800C4A24 = 1;
        break;
    case 1:
        ((void (*)(void))D_800C4A14)();
        if (IsScreenFadeComplete() == 1) {
            D_800C4A20 = 0;
            D_800C4A24 = 2;
        }
        break;
    case 2:
        D_800C4A20++;
        ((void (*)(void))D_800C4A14)();
        if (((s32 *)D_8005E3A8)[2] & 0x840) {
            func_8001FABC(0);
            BeginScreenFadeIncrease(10, 0, 2);
            D_800C4A24 = 3;
        } else if (D_800C4A20 >= 1800) {
            D_800C4A26 = 1;
            D_800C4A24 = 33;
        }
        break;
    case 3:
        ((void (*)(void))D_800C4A14)();
        if (IsScreenFadeComplete() == 1) {
            ovl_27_func_800B7F7C();
            D_800C4A24 = 4;
        }
        break;
    case 4:
        ovl_27_func_800B928C();
        BeginScreenFadeDecrease(10, 0, 2);
        D_800C4A24 = 5;
        break;
    case 5:
        ovl_27_func_800B928C();
        if (IsScreenFadeComplete() == 1) {
            D_800C4A1C = 0;
            D_800C4A24 = 6;
        }
        break;
    case 6:
        ovl_27_func_800B928C();
        if (D_800C4A1C >= 1800) {
            D_800C4A26 = 1;
            D_800C4A24 = 33;
        }
        if (((s32 *)D_8005E3A8)[2] & 0x800) {
            func_8001FABC(0);
            ovl_27_func_800B7F7C();
            D_800C4A24 = 7;
        }
        break;
    case 7:
        ovl_27_func_800B9224();
        if (((s32 *)D_8005E3A8)[2] & 0x5100) {
            func_8001FABC(5);
            D_800C4A1A++;
            D_800C4A1A -= (D_800C4A1A / 2) * 2;
        } else if (((s32 *)D_8005E3A8)[2] & 0x840) {
            func_8001FABC(0);
            if (D_800C4A1A == 0) {
                D_800C4A24 = 8;
            } else {
                BeginScreenFadeIncrease(10, 0, 2);
                func_8001FE34(10);
                D_800C4A24 = 9;
            }
        }
        break;
    case 8:
        ovl_27_func_800B9224();
        D_800C4A24 = 11;
        break;
    case 9:
        ovl_27_func_800B9224();
        if (IsScreenFadeComplete() == 1) {
            D_800C4A24 = 10;
        }
        break;
    case 10:
        if (func_8001FE6C() == 0) {
            s32 *counterBase;
            s32 count;

            counterBase = (s32 *)&D_8006C838;
            count = counterBase[0x4488 / 4] + 1;
            D_800C4A24 = 0;
            counterBase[0x4488 / 4] = count;
        }
        break;
    case 11:
        ovl_27_func_800B92E4();
        func_8002261C(3, 0x3A2);
        D_800C4A24 = 12;
        break;
    case 12:
        ovl_27_func_800B92E4();
        if (GetVal8005E5B4() == 2) {
            D_800C4A24 = 13;
        }
        break;
    case 13:
        ovl_27_func_800B92E4();
        func_80023100();
        func_8002261C(3, 0x397);
        D_800C4A24 = 14;
        break;
    case 14:
        ovl_27_func_800B92E4();
        if (GetVal8005E5B4() == 2) {
            func_80023170((u16 *)&D_80071A00);
            D_800C4A24 = 15;
        }
        break;
    case 15:
        ovl_27_func_800BA130(0);
        ovl_27_func_800B92E4();
        if (func_800231E8() == 1) {
            D_800C4A24 = 16;
        }
        break;
    case 16:
        ovl_27_func_800B92E4();
        func_8002301C();
        func_8002261C(3, 0x3A5);
        D_800C4A24 = 17;
        break;
    case 17:
        ovl_27_func_800B92E4();
        if (GetVal8005E5B4() == 2) {
            func_80023030((s32)D_80071A7C);
            D_800C4A24 = 18;
        }
        break;
    case 18:
        ovl_27_func_800BA130(1);
        ovl_27_func_800B92E4();
        switch (func_80023060()) {
        case -1:
            D_800C4A24 = 13;
            break;
        case 1:
            D_800C4A24 = 19;
            break;
        }
        break;
    case 19:
        ovl_27_func_800B92E4();
        func_80023100();
        func_8002261C(3, 0x3A4);
        D_800C4A24 = 20;
        break;
    case 20:
        ovl_27_func_800B92E4();
        if (GetVal8005E5B4() == 2) {
            func_80023130((u16 *)&D_80070D18);
            D_800C4A24 = 21;
        }
        break;
    case 21:
        ovl_27_func_800BA130(2);
        ovl_27_func_800B92E4();
        switch (func_800231E8()) {
        case -1:
            D_800C4A24 = 16;
            break;
        case 1:
            D_800C4A24 = 22;
            break;
        }
        break;
    case 22:
        ovl_27_func_800B92E4();
        func_80023100();
        func_8002261C(3, 0x3A3);
        D_800C4A24 = 23;
        break;
    case 23:
        ovl_27_func_800B92E4();
        if (GetVal8005E5B4() == 2) {
            func_800231AC((u16 *)&D_800759E8);
            D_800C4A24 = 24;
        }
        break;
    case 24:
        ovl_27_func_800BA130(3);
        ovl_27_func_800B92E4();
        switch (func_800231E8()) {
        case -1:
            D_800C4A24 = 19;
            break;
        case 1:
            D_800C4A24 = 25;
            break;
        }
        break;
    case 25:
        ovl_27_func_800B92E4();
        ovl_27_func_800B9124();
        func_80023100();
        func_8002261C(3, 0x3AA);
        D_800C4A24 = 26;
        break;
    case 26:
        ovl_27_func_800B92E4();
        ovl_27_func_800B9124();
        if (GetVal8005E5B4() == 2) {
            if ((s16)GetVal8005E5B8() == 1) {
                BeginScreenFadeIncrease(10, 0, 2);
                func_8001FE34(10);
                D_800C4A24 = 27;
            } else {
                D_800C4A24 = 13;
            }
        }
        break;
    case 27:
        ovl_27_func_800B92E4();
        if (IsScreenFadeComplete() == 1) {
            D_800C4A24 = 28;
        }
        break;
    case 28:
        if (func_8001FE6C() == 0) {
            s32 *counterBase;

            D_800C4A24 = 0;
            counterBase = (s32 *)&D_8006C838;
            counterBase[3] |= 0x2000;
            counterBase[0x4488 / 4]++;
        }
        break;
    case 29:
        if (((s32 *)D_8005E3A8)[2] & 0x800) {
            ovl_27_func_800B7F7C();
            D_800C4A24 = 4;
        }
        break;
    case 33:
        ((void (*)(void))D_800C4A14)();
        BeginScreenFadeIncrease(10, 0, 2);
        func_8001FE34(10);
        D_800C4A24 = 34;
        break;
    case 34:
        ((void (*)(void))D_800C4A14)();
        if (IsScreenFadeComplete() == 1) {
            D_800C4A24 = 35;
        }
        break;
    case 35:
        if (func_8001FE6C() == 0) {
            base = (s32 *)&D_8006C838;
            if (base[3] & 0x4000) {
                base[3] &= ~0x4000;
            }
            D_800C4A24 = 0;
            base[0x4488 / 4]++;
        }
        break;
    }
}
