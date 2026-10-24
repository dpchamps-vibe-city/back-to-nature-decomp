#include "common.h"
#include "game_types.h"

void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void SetVal8005E334(s32 arg0);
void func_80022738(void);
void func_80023100(void);
void func_800231AC(u16 *arg0);
s32 func_800231E8(void);
u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
s32 *ovl_11_func_800D0C34(void);
s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_800DF0F8(s32 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_80113CD0(void);
void ovl_11_func_801152BC(void *arg0);
s32 ovl_11_func_80116980(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80117178(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);
s16 ovl_11_func_8011D9DC(s16 arg0, s32 arg1);
s32 ovl_11_func_8011DAD4(s16 id, s16 amount, s32 mode);

s32 ovl_11_func_80114604(void) {
    Ovl11D8012D7B8 params;
    s32 result;
    s32 selected;
    s32 i;
    u16 *dest;
    Ovl11D8012D538 *state;
    struct struct_8006C838_800E8F78 *resident;

    result = 0;
    resident = (struct struct_8006C838_800E8F78 *)D_8006C838;
    params.unk4 = 2;
    params.unk6 = 0x62;
    params.unk8 = 2;
    params.unkA = 0x6B;
    params.unkC = 2;
    params.unkE = 0x6E;
    params.unk10 = 4;
    params.unk0 = D_8012D548;
    resident->field_522C = 0;
    resident->field_5230 = 0;
    resident->field_5234 |= 0x800000;
    switch (D_8012D524) {
    case 0:
        ovl_11_func_8011D084(2, 0);
        break;
    case 1:
        ovl_11_func_80113CD0();
        D_8012D52C = 4;
        params.unk4 = -1;
        ovl_11_func_8011D084(2, 0);
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        switch (ovl_11_func_8011B840(&D_8012D524, 2, params)) {
        case 0:
        case 2:
            break;
        case 3:
            ovl_11_func_8011D084(27, 0);
            break;
        case 1:
            switch (((StructD548 *)D_8012D548)->index) {
            case 0:
                if (((Ovl11SaleGlobals *)D_8006C838)->amount >= 1000) {
                    func_8002261C(2, 0x6D);
                    ovl_11_func_8011D084(25, 1);
                } else {
                    func_8002261C(2, 0x6C);
                    ovl_11_func_8011D084(25, 9);
                }
                break;
            case 1:
            case 2:
            case 7:
            case 8:
                if (ovl_11_func_8011D06C(ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)D_8012D548))) == 1) {
                    if (ovl_11_func_8011D9DC(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), 1) == 0) {
                        func_8002261C(2, 0x54);
                        ovl_11_func_8011D084(25, 24);
                    } else {
                        ovl_11_func_8011DAD4(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), 1, 1);
                        ovl_11_func_800F2354(-ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)D_8012D548)), 1);
                        func_8002261C(2, 0x84);
                        ovl_11_func_8011D084(25, 24);
                    }
                } else {
                    func_8002261C(2, 0x4F);
                    ovl_11_func_8011D084(25, 24);
                }
                break;
            case 3:
            case 4:
                if (ovl_11_func_800D0C34() == 0) {
                    func_8002261C(2, 0x72);
                    ovl_11_func_8011D084(25, 24);
                } else if (ovl_11_func_8011D06C(ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)D_8012D548))) != 1) {
                    func_8002261C(2, 0x4F);
                    ovl_11_func_8011D084(25, 24);
                } else {
                    func_8002261C(2, 0x73);
                    ovl_11_func_8011D084(25, 16);
                }
                break;
            case 5:
                if (ovl_11_func_80117178(1, 0, -1) == 0) {
                    func_8002261C(2, 0x76);
                    ovl_11_func_8011D084(25, 24);
                } else {
                    func_8002261C(2, 0x75);
                    D_8012D52C = 0;
                    ovl_11_func_80116980(0, 0, -1);
                    ovl_11_func_8011D084(25, 18);
                }
                break;
            case 6:
                if (ovl_11_func_80117178(2, 0, -1) == 0) {
                    func_8002261C(2, 0x78);
                    ovl_11_func_8011D084(25, 24);
                } else {
                    func_8002261C(2, 0x77);
                    D_8012D52C = 0;
                    ovl_11_func_80116980(0, 0, -1);
                    ovl_11_func_8011D084(25, 21);
                }
                break;
            }
            break;
        }
        break;
    case 9:
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(2, 0x4D);
        ovl_11_func_8011D084(26, 10);
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        state = &D_8012D538;
        switch (ovl_11_func_8011BAFC(&D_8012D524, 10, 5, *(Ovl11BAFCParams *)&params, (ovl_11_DD48_arg0 *)state)) {
        case 0:
        case 2:
            break;
        case 3:
            ovl_11_func_8011D084(25, 24);
            break;
        case 1:
            func_80022738();
            if (ovl_11_func_8011D06C(state->unk8 * state->unkA) == 1) {
                if (state->unk8 + ((Ovl11SaleGlobals *)D_8006C838)->amount >= 1000) {
                    func_8002261C(2, 0x6D);
                    ovl_11_func_8011D084(25, 9);
                } else {
                    ovl_11_func_801152BC(state);
                    func_8002261C(2, 0x70);
                    ovl_11_func_8011D084(25, 24);
                }
            } else {
                func_8002261C(2, 0x4F);
                ovl_11_func_8011D084(25, 9);
            }
            break;
        }
        break;
    case 16:
        func_80023100();
        func_800231AC(D_8012D63C);
        D_8012D52C = 0;
        ovl_11_func_8011D084(17, 0);
        break;
    case 17:
        switch (func_800231E8()) {
        case -1:
            func_8002261C(2, 0x6E);
            ovl_11_func_8011D084(25, 24);
            break;
        case 1:
            dest = ovl_11_func_800CE744(ovl_11_func_8011D98C(*(StructD548 *)D_8012D548), -1) + 2;
            for (i = 0; i < 9; i++) {
                dest[i] = D_8012D638.values[i];
            }
            ovl_11_func_800F2354(-ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)D_8012D548)), 1);
            func_8002261C(2, 0x74);
            ovl_11_func_8011D084(25, 24);
            break;
        }
        break;
    case 18:
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(2, 0x5A);
        ovl_11_func_8011D084(26, 19);
        break;
    case 19:
        selected = ovl_11_func_80116980(1, 0, 0);
        if (selected == -2) {
            func_80022738();
            func_8002261C(2, 0x79);
            ovl_11_func_8011D084(25, 24);
        } else if (selected != -1) {
            func_80022738();
            SetVal8005E334(0);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                func_8002261C(2, 0x82);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                func_8002261C(2, 0x7D);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                func_8002261C(2, 0x7F);
            } else {
                func_8002261C(2, 0x80);
            }
            ovl_11_func_8011D084(26, 20);
        }
        break;
    case 20:
        selected = ovl_11_func_80116980(1, 0, 1);
        switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            func_80022738();
            func_8002261C(2, 0x83);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                ovl_11_func_800F2354(8000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                ovl_11_func_800F2354(3000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                ovl_11_func_800F2354(4500, 1);
            } else {
                ovl_11_func_800F2354(5000, 1);
            }
            ovl_11_func_800DF0F8((s32)((u8 *)&D_800749F4 + selected * 0xB8));
            ovl_11_func_8011D084(25, 24);
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0x79);
            ovl_11_func_8011D084(25, 24);
            break;
        }
        break;
    case 21:
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(2, 0x5A);
        ovl_11_func_8011D084(26, 22);
        break;
    case 22:
        selected = ovl_11_func_80116980(2, 0, 0);
        if (selected == -2) {
            func_80022738();
            func_8002261C(2, 0x79);
            ovl_11_func_8011D084(25, 24);
        } else if (selected != -1) {
            func_80022738();
            SetVal8005E334(0);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                func_8002261C(2, 0x81);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                func_8002261C(2, 0x7C);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                func_8002261C(2, 0x7D);
            } else {
                func_8002261C(2, 0x7E);
            }
            ovl_11_func_8011D084(26, 23);
        }
        break;
    case 23:
        selected = ovl_11_func_80116980(2, 0, 1);
        switch (ovl_11_func_80118B84(D_8005E3C0->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            func_80022738();
            func_8002261C(2, 0x83);
            if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].flags & 0x40) {
                ovl_11_func_800F2354(6000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 100) {
                ovl_11_func_800F2354(2000, 1);
            } else if (((Ovl11SaleAnimalGlobals *)D_8006C838)->animals[selected].affection <= 200) {
                ovl_11_func_800F2354(3000, 1);
            } else {
                ovl_11_func_800F2354(3500, 1);
            }
            ovl_11_func_800DF0F8((s32)((u8 *)&D_800749F4 + selected * 0xB8));
            ovl_11_func_8011D084(25, 24);
            break;
        case 2:
            func_80022738();
            func_8002261C(2, 0x79);
            ovl_11_func_8011D084(25, 24);
            break;
        }
        break;
    case 24:
        func_8002261C(2, 0x71);
        ovl_11_func_8011D084(25, 1);
        break;
    case 25:
        if (GetVal8005E5B4() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 26:
        if (D_8012D528 == 20) {
            ovl_11_func_80116980(1, 0, 1);
        }
        if (D_8012D528 == 23) {
            ovl_11_func_80116980(2, 0, 1);
        }
        if (GetVal8005E5B4() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 27:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
