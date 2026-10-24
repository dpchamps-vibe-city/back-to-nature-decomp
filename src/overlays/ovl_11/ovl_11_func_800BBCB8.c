#include "common.h"

void func_80011EF0(s32 arg0);
/* Overlay names for the matched setters at 80011F44 and 80011F50. */
void SetVal8005E278(s32 arg0);
void SetVal8005E27C(s32 arg0);
void func_800132F0(s32 arg0, s32 arg1, s32 arg2);
void func_80013328(s16 arg0);
s32 func_80013394(void);
s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);
s32 func_8001FABC(s16 arg0);
s32 func_8001FBBC(s16 arg0);
void func_8001FE34(s32 arg0);
void ovl_11_func_800BCF28(void);
s32 ovl_11_func_800BD374(s32 arg0, s32 arg1);
s32 ovl_11_func_800CB4E0(void);
void ovl_11_func_800CD624(void);
void ovl_11_func_800CD670(s16 arg0);
void ovl_11_func_800DBAB0(void);
s32 ovl_11_func_800DC388(s32 arg0);
s32 ovl_11_func_800DE2EC(void);
void ovl_11_func_800F4E84(void);

#define WORK ((Ovl11BBCB8Work *)&D_8007AFF0)
#define STATUS ((Ovl11BBCB8Status *)D_8006C838)

void ovl_11_func_800BBCB8(void) {
    u16 selection;

    switch (WORK->state) {
    case 26:
        WORK->state = 27;
        func_800132F0(10, 0, 2);
        func_8001FE34(10);
        break;
    case 27:
        if (func_80013394() == 1) {
            func_80011EF0(13);
        }
        break;
    case 0:
        D_801217A0 = 0;
        break;
    case 1:
        if (!(STATUS->objectFlags & 8)) {
            WORK->state = 2;
            if (D_801287F0->unk2 != 0xFFFE) {
                ovl_11_func_800CD670(20);
            } else {
                ovl_11_func_800CD670(27);
            }
        } else if (STATUS->objectFlags & 0x800000) {
            WORK->state = 3;
            STATUS->objectFlags |= 0x400;
            STATUS->flags |= 0x8000000;
        }
        break;
    case 2:
        if (!(STATUS->objectFlags & 8)) {
            if (D_801287F0 != 0) {
                if (D_801287F0->unk2 == 0xFFFE) {
                    WORK->state = 0;
                    if (D_801287F0->unkA == 0x2E) {
                        STATUS->selectionFlags |= 0x80;
                    }
                    if (D_801287F0->unkA == 0x20 && D_801287F0->unk8 == 2) {
                        STATUS->selectionFlags |= 0x20;
                    }
                } else {
                    WORK->state = 3;
                    STATUS->objectFlags |= 0x400;
                    STATUS->flags |= 0x8000000;
                }
            } else {
                WORK->state = 0;
            }
        }
        break;
    case 3:
        ovl_11_func_800CB4E0();
        func_80013328(20);
        ovl_11_func_800F4E84();
        D_801287EC = 0;
        if (D_801287F0->unkA != 99) {
            if (!(D_801287F0->unk0 & 0x200)) {
                D_801287EC = 1;
            }
        }
        selection = D_801287F0->unk2;
        WORK->object = D_801287F0;
        WORK->state = 5;
        STATUS->objectFlags &= ~0x400;
        WORK->savedSelection = selection;
        D_801287E8 = ovl_11_func_800DC388(selection);
        ovl_11_func_800BD374(D_801287E8, 1);
        D_801217A0 = 1;
        if (STATUS->mode == 2) {
            ovl_11_func_800DBAB0();
        }
        break;
    case 24:
        ovl_11_func_800CB4E0();
        WORK->object = 0;
        WORK->state = 25;
        STATUS->flags |= 0x8000000;
        D_801287E8 = ovl_11_func_800DC388(WORK->savedSelection);
        ovl_11_func_800BD374(D_801287E8, 1);
        D_801217A0 = 1;
        break;
    case 13:
    case 30:
        if (func_80013394() == 1) {
            WORK->state = 0;
        }
        break;
    case 5:
    case 25:
        STATUS->flags |= 0x800000;
        if (D_801217A0 != 0 && ovl_11_func_800BD374(D_801287E8, 0) == 0) {
            break;
        }
    case 4:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 28:
        STATUS->flags |= 0x800000;
        STATUS->flags &= ~0x10000000;
    case 12:
    case 31:
        if (func_80013394() == 1) {
            if (STATUS->mode == 2) {
                ovl_11_func_800DBAB0();
            }
            SetVal8005E278(1);
            SetVal8005E27C(1);
            STATUS->flags &= ~0x800000;
            switch (WORK->state) {
            case 28:
                func_80011EF0(16);
                break;
            case 14:
                func_80011EF0(7);
                break;
            case 15:
                func_80011EF0(8);
                break;
            case 16:
                func_80011EF0(9);
                break;
            case 17:
                func_80011EF0(10);
                break;
            case 18:
                func_80011EF0(3);
                func_8001FBBC(0);
                break;
            case 19:
                func_80011EF0(4);
                break;
            case 20:
                func_80011EF0(5);
                break;
            case 21:
                func_80011EF0(11);
                break;
            case 22:
                func_80011EF0(12);
                break;
            case 12:
            case 31:
                STATUS->objectFlags |= 0x400000;
                ovl_11_func_800BCF28();
                if (WORK->state == 31) {
                    WORK->state = 30;
                    func_8001AF70(0x9D, 1);
                    func_8001AF70(0x47, 1);
                } else {
                    WORK->state = 13;
                }
                ovl_11_func_800CD624();
                break;
            case 5:
                if (D_801287EC != 0) {
                    func_8001FABC(0x4C);
                }
            case 25:
                D_801287F4 = 3;
                STATUS->objectFlags &= ~0x402;
                STATUS->counter--;
                WORK->selection = WORK->savedSelection;
                break;
            case 4:
                D_801287F4 = 3;
                STATUS->counter--;
                WORK->selection++;
                if (WORK->selection == 256) {
                    WORK->selection = 0;
                }
                if (func_8001AF44(0x9C) == 1) {
                    WORK->selection = D_8005E32C;
                    func_8001AF70(0x9C, 0);
                }
                STATUS->direction = WORK->selection - WORK->selection / 4 * 4;
                ovl_11_func_800DE2EC();
                WORK->object = 0;
                break;
            }
        }
        break;
    case 23:
        if (func_80013394() == 1) {
            WORK->pendingMode = 3;
            WORK->state = 0;
        }
        break;
    case 6:
        if (func_80013394() == 1) {
            WORK->state = 0;
            func_80011EF0(17);
        }
        break;
    }
}
