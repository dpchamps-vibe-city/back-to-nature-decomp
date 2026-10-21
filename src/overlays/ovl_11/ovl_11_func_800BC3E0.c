#include "common.h"
#include "game_types.h"

void ovl_11_func_801044E4(void);
void ovl_11_func_800BD160(void);
void ovl_11_func_800BD168();
void func_8001AF70(u16 arg0, u16 arg1);
s32 func_8001FD74(void);
void func_8001E160(void);
void func_8001B530(void);
void ovl_11_func_800BCD28(void);
void ovl_11_func_800BC868(void);
/* Overlay import of ClearVal8005E2CC at 8001B100. */
void func_8001B100(void);
s32 func_80020818(void);
void func_8001FE00(s32 arg0);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_800CD0C0(s32 *arg0);
s32 func_8001AF44(u32 arg0);
void ovl_11_func_800CCC5C(void);
void ovl_11_func_800BC9AC(void);
void ovl_11_func_800CF36C(s32 arg0);
void ovl_11_func_800CF428(void);
void ovl_11_func_800C1C90(void);
void ovl_11_func_800F4114(void);
void ovl_11_func_80121318(void);
void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
void ovl_11_func_800DBAB0(void);
u32 func_80012A34(s32 arg0);
/* The callee stores argument 1 as a halfword; no caller-side narrowing. */
void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2);
void ovl_11_func_800F1038(u16 arg0, s32 arg1);

void ovl_11_func_800BC3E0(void) {
    s32 value;
    Ovl11AreaInitView *state;

    switch (D_801287F4) {
    case 0:
        ovl_11_func_801044E4();
    case 9:
        ((Ovl11MovementState *)D_8006C838)->flagsC |= 0x8000000;
        ((Ovl11MovementState *)D_8006C838)->flags10 |= 0x8000000;
        ovl_11_func_800BD160();
        func_8001AF70(3, 1);
        D_801287F4++;
        break;
    case 1:
        ovl_11_func_800BD168();
        D_801287F4++;
        break;
    case 2:
        state = (Ovl11AreaInitView *)D_8006C838;
        if (state->flags & 0x2000) {
            ((Ovl11FarInitView *)&D_8007AFF0)->field_2547C = 0;
        } else {
            s32 offset;

            ((Ovl11FarInitView *)&D_8007AFF0)->field_2547C =
                state->field_44BA + (offset = state->field_44CE * 4 + 0x50);
        }
    case 10:
        if (func_8001FD74()) {
            D_801287F4++;
            func_8001E160();
            func_8001B530();
        }
        break;
    case 3:
    case 11:
        D_801287F4++;
        break;
    case 4:
    case 12:
        ovl_11_func_800BCD28();
        D_801287F4++;
        break;
    case 5:
    case 13:
        D_801217A4 = 0;
        ovl_11_func_800BC868();
        D_801287F4++;
        break;
    case 6:
    case 14:
        if (func_8001FD74()) {
            D_801287F4++;
        }
        break;
    case 7:
    case 15:
        func_8001B100();
        if (D_801217A4 && !(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 0x4000)) {
            func_80020818();
            func_8001FE00(10);
        }
        ((Ovl11AreaInitView *)D_8006C838)->field_4450 &= ~0x4000;
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0x19) {
            func_800132B8(10, 0, 2);
        }
        ((Ovl11AreaInitView *)D_8006C838)->field_4488++;
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 == 0xC) {
            ovl_11_func_800CD0C0((s32 *)((struct Ovl11FrameState *)D_8006C838)->position);
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[0] = ((struct Ovl11FrameState *)D_8006C838)->position[0];
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[1] = ((struct Ovl11FrameState *)D_8006C838)->position[2];
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[2] = ((struct Ovl11FrameState *)D_8006C838)->position[4];
        }
        if (!(((Ovl11MovementState *)D_8006C838)->flagsC & 0x2000)) {
            if (!func_8001AF44(2)) {
                func_8001AF70(3, 0);
            }
        } else {
            func_8001AF70(3, 1);
        }
        ovl_11_func_800CCC5C();
        ovl_11_func_800BC9AC();
        ovl_11_func_800CF36C(0);
        ovl_11_func_800CF428();
        ovl_11_func_800C1C90();
        ovl_11_func_800F4114();
        ovl_11_func_80121318();
        if (((Ovl11AreaInitView *)D_8006C838)->field_4450 & 0x40) {
            ((Ovl11AreaInitView *)D_8006C838)->field_4450 &= ~0x40;
            if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == 2) {
                ovl_11_func_800F0FB0(8);
                ovl_11_func_800F0F50();
                ovl_11_func_800DBAB0();
                if (func_8001AF44(2) == 1) {
                    for (;;) {
                        if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                            ovl_11_func_800DBAB0();
                            break;
                        }
                        ovl_11_func_800DBAB0();
                        if (func_8001AF44(2) == 0) {
                            break;
                        }
                    }
                }
            }
            if (!(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 4) && !func_8001AF44(2)) {
                ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11AreaInitView *)D_8006C838)->field_51C8, func_80012A34(4) + 0x11B, 0);
            }
        } else {
            if (((Ovl11MovementState *)D_8006C838)->flagsC & 0x2000) {
                ovl_11_func_800F1038(0x74, 1);
                value = ((Ovl11MovementState *)D_8006C838)->flagsC & ~0x2000;
                ((Ovl11MovementState *)D_8006C838)->flagsC = value;
            } else {
                ovl_11_func_800F0FB0(2);
            }
            ovl_11_func_800F0F50();
            ovl_11_func_800DBAB0();
            if (func_8001AF44(2) == 1) {
                for (;;) {
                    if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                        ovl_11_func_800DBAB0();
                        break;
                    }
                    ovl_11_func_800DBAB0();
                    if (func_8001AF44(2) == 0) {
                        break;
                    }
                }
            }
        }
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0x18) {
            ((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 = 0;
            value = ((Ovl11MovementState *)D_8006C838)->flagsC & ~0x8000000;
            ((Ovl11MovementState *)D_8006C838)->flagsC = value;
        }
        break;
    }
}
