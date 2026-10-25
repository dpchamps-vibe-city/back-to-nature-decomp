#include "common.h"

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226B0(void);
s32 func_8001FABC(s16 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void func_80022738(void);
void ovl_11_func_80102368(void);
void ovl_11_func_801024D8(void);
void ovl_11_func_80102268(s8 arg0, s8 arg1);
void ovl_11_func_801022E8(void);
s32 ovl_11_func_801027D4(s8 arg0, s8 arg1);
void *ovl_11_func_80102844(s8 arg0, s8 arg1);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_80100FFC(void);

void ovl_11_func_80101B9C(void) {
    s32 ui;
    s32 text;
    u32 *panels;
    s16 empty;
    s32 i;

    ui = gActiveRenderContext->field_D8;
    panels = (u32 *)(ui + 0x74);
    text = ui + 0x70;
    func_80022580(panels, 1, 0x16, 0x10, 0x10E, 0x4A);
    func_80022580(panels, 1, 0x16, 0x5A, 0x10E, 0x4A);
    func_80017B3C(text, (s32)(D_800539E8 + D_80054BBC[0]), 0x21, 0x20);
    if (D_801273EA_table == 0) {
        func_80017B3C(text, (s32)(D_800539E8 + 0x16 + D_80054BBC[0]), 0x21, 0x6A);
    } else {
        func_80017B3C(text, (s32)(D_800539E8 + 0x28 + D_80054BBC[0]), 0x21, 0x6A);
        func_8001AAF4(D_801273EA_table, 1, text, 0x2A, 0x78);
        func_80017B3C(text, (s32)(D_800539E8 + 0x36 + D_80054BBC[0]), 0x33, 0x78);
        func_8001AAF4(8, 1, text, 0x3C, 0x78);
    }
    ovl_11_func_80102368();
    ovl_11_func_801024D8();
    empty = 8;
    for (i = 0; i < 8; i++) {
        if (D_8012CF00_ids[i] == 0) {
            empty = i;
            break;
        }
    }
    if (D_801273E4_state == 6) {
        ovl_11_func_80102268(Ovl11_D_801273E7, D_801273EA_table);
    } else if (D_801273E4_state == 7) {
        ovl_11_func_801022E8();
    } else {
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(3, 0x3BE);
        if (func_800226B0()) {
            switch (((u32 *)D_8005E3A8)[0]) {
            case 0x1000:
            case 0x4000:
                Ovl11_D_801273E7 = Ovl11_D_801273E7 >= 4 ?
                    Ovl11_D_801273E7 - 4 : Ovl11_D_801273E7 + 4;
                func_8001FABC(5);
                break;
            case 0x2000:
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == 7) {
                    D_801273EA_table++;
                }
                Ovl11_D_801273E7++;
                if (Ovl11_D_801273E7 == Ovl11_D_801273E7 / 4 * 4) {
                    Ovl11_D_801273E7 -= 4;
                }
                func_8001FABC(5);
                break;
            case 0x8000:
                if (Ovl11_D_801273E7 == 0 || Ovl11_D_801273E7 == 4) {
                    D_801273EA_table--;
                }
                Ovl11_D_801273E7--;
                if (Ovl11_D_801273E7 == 3 || Ovl11_D_801273E7 == -1) {
                    Ovl11_D_801273E7 += 4;
                }
                func_8001FABC(5);
                break;
            }
            if (((u32 *)D_8005E3A8)[0] & 0xA) {
                D_801273EA_table++;
                func_8001FABC(5);
            }
            if (((u32 *)D_8005E3A8)[0] & 5) {
                D_801273EA_table--;
                func_8001FABC(5);
            }
            if (D_801273EA_table < 0) {
                D_801273EA_table = 8;
            }
            if (D_801273EA_table > 8) {
                D_801273EA_table = 0;
            }
        }
        func_800248B0(gActiveRenderContext->field_D8 + 0x54,
                     (Ovl11_D_801273E7 - Ovl11_D_801273E7 / 4 * 4) * 34 + 0x87,
                     Ovl11_D_801273E7 / 4 * 34 + 0x67);
        if (func_800226B0()) {
            if (((u32 *)D_8005E3A8)[2] & 0x800) {
                for (i = 0; i < 8; i++) {
                    if (D_8012CF00_ids[i] != 0) {
                        func_80022738();
                        func_8001FABC(0);
                        D_801273E4_state = 7;
                        return;
                    }
                }
                func_8001FABC(2);
            } else if (((u32 *)D_8005E3A8)[2] & 0x10) {
                if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                    func_80022738();
                    func_8001FABC(0);
                    D_801273E4_state = 6;
                } else {
                    func_8001FABC(2);
                }
            } else if (((u32 *)D_8005E3A8)[2] & 0x20) {
                func_8001FABC(1);
                if (*(u16 *)&D_801273F0 != 0) {
                    for (i = 0; i < 8; i++) {
                        D_8012CF00_ids[i] = 0;
                    }
                } else {
                    func_80022738();
                    Ovl11_D_801273DC = ovl_11_func_80100FFC;
                }
            } else if (((u32 *)D_8005E3A8)[2] & 0x40) {
                if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) == 0) {
                    func_8001FABC(2);
                    return;
                }
                if (ovl_11_func_801027D4(D_801273EA_table, Ovl11_D_801273E7) == 1) {
                    for (i = 0; i < 8; i++) {
                        if (D_8012CF00_ids[i] == *(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7)) {
                            func_8001FABC(1);
                            D_8012CF00_ids[i] = 0;
                            break;
                        }
                    }
                } else {
                    if (*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7) != 0) {
                        if (empty < 8 && ovl_11_func_800D583C(*(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7)) != 1) {
                            func_8001FABC(0);
                            D_8012CF00_ids[empty] = *(s16 *)ovl_11_func_80102844(D_801273EA_table, Ovl11_D_801273E7);
                        } else {
                            func_8001FABC(2);
                        }
                    } else {
                        func_8001FABC(2);
                    }
                }
            }
        }
    }
}
