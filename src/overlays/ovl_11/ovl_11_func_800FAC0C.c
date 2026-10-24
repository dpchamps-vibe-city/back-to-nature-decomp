#include "common.h"

void func_80017A38(s16, s16);
void SetVal8005E2BC(s32);
void func_80017B3C(s32, s32, s32, s32);
s32 pow_int(s32, s32);
s32 func_8001FABC(s16);
void func_80022580(u32 *, s32, s32, s32, s16, s16);
s32 func_8002261C(s32, s32);
s32 func_800226B0(void);
void SetVal8005E334(s32);
void func_80022738(void);
void func_800248B0(s32, s16, s16);
void ovl_11_func_800F6F7C(s32, s16, s16);
void ovl_11_func_800FB0C4(s16 *);
s32 ovl_11_func_800FB120(s32, s32);
char *ovl_11_func_800FB4B0(s32);
void ovl_11_func_800FB510(s32, s32, s16 *);

void ovl_11_func_800FAC0C(void) {
    s32 i;
    s32 text;
    s32 items;
    s32 direction;
    s32 state;
    u32 buttons;
    u32 movement;
    s32 failed;

    text = D_8005E3C0->field_D8 + 0x60;
    items = D_8005E3C0->field_D8 + 0x5C;
    func_80017A38(0, 2);
    for (i = 0; i < 4U; i++) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x64), 1,
            D_801271E8[i][0], D_801271E8[i][1], D_801271E8[i][2], D_801271E8[i][3]);
    }
    func_80017B3C(text, (s32)(D_80051B7C + D_80054BBC[0]), 0x1D, 0x14);
    func_80017B3C(text, (s32)(D_80051B7C + 0xC + D_80054BBC[0]), 0x1D, 0x5E);
    ovl_11_func_800FB510(items, 0, (s16 *)((u8 *)D_8006C838 + 0x524C));
    ovl_11_func_800FB510(items, 9, (s16 *)((u8 *)D_8006C838 + 0x5252));
    for (i = 0; i < pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1); i++) {
        ovl_11_func_800FB510(items, i + 1, (s16 *)((u8 *)D_8006C838 + 0x5258 + i * 6));
        ovl_11_func_800FB510(items, i + 10, (s16 *)((u8 *)D_8006C838 + 0x5288 + i * 6));
    }
    direction = 4;
    if (D_80126FEC != 0xFF) {
        ovl_11_func_800F6F7C(D_8005E3C0->field_D8 + 0x54, D_80126FF0[D_80126FEC][0], D_80126FF0[D_80126FEC][1]);
    } else {
        direction = 0;
    }
    state = D_80126FE0;
    if (state != 1) {
        ovl_11_func_800FB0C4((s16 *)ovl_11_func_800FB4B0(D_80126FE4));
        return;
    }
    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x3AC);
    if (func_800226B0()) {
        buttons = ((u32 *)D_8005E3A8)[2];
        if (buttons & 0x10) {
            if (D_80126FE0 == state) {
                if (*(s16 *)ovl_11_func_800FB4B0(D_80126FE4)) {
                    func_8001FABC(0);
                    func_80022738();
                    D_80126FE0 = 2;
                } else {
                    func_8001FABC(2);
                }
            }
        } else if (buttons & 0x20) {
            if (D_80126FEC == 0xFF) {
                func_80022738();
                D_80126FE0 = 3;
                func_8001FABC(3);
                return;
            }
            func_8001FABC(1);
            D_80126FEC = 0xFF;
        } else if (buttons & 0x40) {
            if (D_80126FEC == 0xFF) {
                func_8001FABC(0);
                D_80126FEC = D_80126FE4;
            } else {
                D_80126FE8 = D_80126FE4;
                if (D_80126FEC == D_80126FE4) {
                    D_80126FE4 = D_80126FEC < 9 ? 0 : 9;
                }
                failed = ovl_11_func_800FB120(D_80126FEC, D_80126FE4);
                D_80126FE4 = D_80126FE8;
                if (!failed) {
                    func_8001FABC(0);
                    D_80126FEC = 0xFF;
                } else {
                    func_8001FABC(2);
                }
            }
        } else if (buttons & 0x800) {
            func_80022738();
            func_8001FABC(3);
            D_80126FE0 = 3;
        } else {
            movement = ((u32 *)D_8005E3A8)[0] & 0xF000;
            if (movement) {
                switch (movement) {
                case 0x1000:
                    break;
                case 0x4000:
                    direction++;
                    break;
                case 0x8000:
                    direction += 2;
                    break;
                case 0x2000:
                    direction += 3;
                    break;
                }
                D_80126FE4 = Ovl11FarmMenuRows[D_80126FE4].destinations[((struct Ovl11ItemGridState *)D_8006C838)->exponent + direction * 3];
                func_8001FABC(5);
            }
        }
    }
    func_800248B0(D_8005E3C0->field_D8 + 0x50, (s16)(D_80126FF0[D_80126FE4][0] + 12), (s16)(D_80126FF0[D_80126FE4][1] + 24));
}
