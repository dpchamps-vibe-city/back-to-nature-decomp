#include "common.h"

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800226D8(s32 arg0);
void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226B0(void);
s32 func_8001FABC(s16 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void func_80022738(void);
void ovl_11_func_80101814(void);
void ovl_11_func_80101964(void);
void ovl_11_func_801014A4(s8 arg0);
void ovl_11_func_801014F8(void);
s32 ovl_11_func_80101B28(s8 arg0);
s32 ovl_11_func_80101B84(s8 arg0);
void ovl_11_func_80101B9C(void);
void ovl_11_func_80100194(void);
void ovl_11_func_8010021C(void);

void ovl_11_func_80100FFC(void) {
    s32 ui;
    s32 text;
    u32 *panels;

    ui = D_8005E3C0->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    func_80022580(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    func_80022580(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_8005396A + 0x28 + D_80054BBC[0]), 0x21, 0x20);
    func_80017B3C(text, (s32)(D_8005396A + D_80054BBC[0]), 0x21, 0x6A);
    ovl_11_func_80101814();
    ovl_11_func_80101964();
    if (D_801273E4_state == 4) {
        ovl_11_func_801014A4(Ovl11_D_801273E7);
    } else if (D_801273E4_state == 5) {
        ovl_11_func_801014F8();
    } else {
        func_800226D8(0);
        func_80017B18(0);
        func_8002261C(3, 0x3BC);
        if (func_800226B0()) {
            switch (((u32 *)D_8005E3A8)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                func_8001FABC(5);
                break;
            case 0x2000:
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                func_8001FABC(5);
                break;
            case 0x8000:
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                func_8001FABC(5);
                break;
            }
        }
        func_800248B0(D_8005E3C0->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (func_800226B0()) {
            if (((u32 *)D_8005E3A8)[2] & 0x800) {
                func_80022738();
                func_8001FABC(0);
                D_801273F0 = 0;
                Ovl11_D_801273DC = ovl_11_func_80101B9C;
            } else if (((u32 *)D_8005E3A8)[2] & 0x20) {
                func_8001FABC(1);
                if (D_801273EE_flags != 0) {
                    D_801273EE_flags = 0;
                } else if (D_801273E5_origin == 0) {
                    func_80022738();
                    Ovl11_D_801273DC = ovl_11_func_80100194;
                } else {
                    func_80022738();
                    Ovl11_D_801273DC = ovl_11_func_8010021C;
                }
            } else if (((u32 *)D_8005E3A8)[2] & 0x10) {
                if (D_80070D04 & ovl_11_func_80101B84(Ovl11_D_801273E7)) {
                    func_80022738();
                    func_8001FABC(0);
                    D_801273E4_state = 4;
                } else {
                    func_8001FABC(2);
                }
            } else if (((u32 *)D_8005E3A8)[2] & 0x40) {
                if (ovl_11_func_80101B28(Ovl11_D_801273E7) == 1) {
                    func_8001FABC(1);
                    if (Ovl11_D_801273E7 < 7) {
                        D_801273EE_flags &= ~ovl_11_func_80101B84(Ovl11_D_801273E7);
                    } else {
                        func_80022738();
                        D_801273E4_state = 5;
                    }
                } else {
                    if (D_80070D04 & ovl_11_func_80101B84(Ovl11_D_801273E7)) {
                        func_8001FABC(0);
                        if (Ovl11_D_801273E7 < 7) {
                            D_801273EE_flags |= ovl_11_func_80101B84(Ovl11_D_801273E7);
                        } else {
                            func_80022738();
                            D_801273E4_state = 5;
                        }
                    } else {
                        func_8001FABC(2);
                    }
                }
            }
        }
    }
}
