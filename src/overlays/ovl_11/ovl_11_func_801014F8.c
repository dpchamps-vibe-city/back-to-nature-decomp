#include "common.h"

void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
s32 func_800226B0(void);
s32 func_8001FABC(s16 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void func_80022738(void);
void func_80017D9C(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4);

void ovl_11_func_801014F8(void) {
    s32 *text;

    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x3BD);
    func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0x5C), 1, 0xA6, 0x58, 0x68, 0x48);
    if (func_800226B0()) {
        switch (((u32 *)D_8005E3A8)[0]) {
        case 0x1000:
        case 0x8000:
            D_801273E9_selection += 4;
            func_8001FABC(5);
            break;
        case 0x2000:
        case 0x4000:
            D_801273E9_selection++;
            func_8001FABC(5);
            break;
        }
        D_801273E9_selection -= (s8)(D_801273E9_selection / 5) * 5;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x54, 0xAF, D_801273E9_selection * 14 + 0x5D);
    if (func_800226B0()) {
        if (((u32 *)D_8005E3A8)[2] & 0x20) {
            func_80022738();
            func_8001FABC(1);
            D_801273E4_state = 3;
        } else if (((u32 *)D_8005E3A8)[2] & 0x40) {
            D_801273EE_flags ^= 1 << (D_801273E9_selection + 7);
            if ((D_801273EE_flags >> (D_801273E9_selection + 7)) & 1) {
                func_8001FABC(0);
            } else {
                func_8001FABC(1);
            }
        }
    }
    text = (s32 *)(gActiveRenderContext->field_D8 + 0x58);
    func_80017D9C(text, (u16 *)(D_8005399A + D_80054BBC[0]), 0xB1, 0x5A, ((D_801273EE_flags >> 7) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0xC + D_80054BBC[0]), 0xB1, 0x68, ((D_801273EE_flags >> 8) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x16 + D_80054BBC[0]), 0xB1, 0x76, ((D_801273EE_flags >> 9) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x26 + D_80054BBC[0]), 0xB1, 0x84, ((D_801273EE_flags >> 10) ^ 1) & 1);
    func_80017D9C(text, (u16 *)(D_8005399A + 0x38 + D_80054BBC[0]), 0xB1, 0x92, ((D_801273EE_flags >> 11) ^ 1) & 1);
}
