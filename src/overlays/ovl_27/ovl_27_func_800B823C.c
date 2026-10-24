#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libsnd.h"
#include "psyq/libgpu.h"


short SsSeqOpen (unsigned long *, short);
void SsSetMono (void);
void SsSetStereo (void);
void SsStart (void);
void SsUtReverbOn (void);
void ovl_27_func_800B9324 (void);
s32 func_80013394 (void);
void ovl_27_func_800BA5BC (void);
s32 ovl_27_func_800BA620 (void);
void ovl_27_func_800BA750 (void);
s32 ovl_27_func_800BA7B0 (void);
void ovl_27_func_800BA514 (void);
s32 ovl_27_func_800BA578 (void);
void ovl_27_func_800BA80C (void);
s32 ovl_27_func_800BA814 (void);
void func_800132F0 (s32 arg0, s32 arg1, s32 arg2);
void ovl_27_func_800BA4C4 (void);
void func_8001FD10 (void);
s32 func_8001FD74 (void);
void func_8001FE00 (s32 arg0);
s32 func_8001FE6C (void);
s32 func_80020818 (void);
s32 func_8001FB30 (s16 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 func_800201C4 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);
s32 func_80020E38 (void);
s32 func_80020E58 (void);
void func_8001FD84 (void);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_8001FBE4 (s32 arg0, s32 arg1);
void func_8001FBF0(s16 arg0, s16 arg1);
void func_8001719C (u8 *arg0);

void ovl_27_func_800B823C(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    env_struct_0x134 *second;
    Ovl27StateCounterView *state;

    if (!(D_800C4A28 & 0x4000)) {
        ovl_27_func_800B9324();
    }
    switch (D_800C4A18) {
    case 0:
        temp_v0 = func_80013394();
        if (temp_v0 == 1) {
            D_800C4A18 = (s16) temp_v0;
            return;
        }
    default:
        return;
    case 1:
        ovl_27_func_800BA5BC();
        D_800C4A18 = 2;
        return;
    case 2:
        if (ovl_27_func_800BA620() != 0) {
            D_800C4A18 = 3;
            return;
        }
        break;
    case 3:
        ovl_27_func_800BA750();
        D_800C4A18 = 4;
        return;
    case 4:
        if (ovl_27_func_800BA7B0() != 0) {
            D_800C4A18 = 5;
            return;
        }
        break;
    case 5:
        ovl_27_func_800BA514();
        D_800C4A18 = 6;
        return;
    case 6:
        if (ovl_27_func_800BA578() != 0) {
            D_800C4A18 = 0xA;
            return;
        }
        break;
    case 10:
        ovl_27_func_800BA80C();
        D_800C4A18 = 0xB;
        return;
    case 11:
        if (ovl_27_func_800BA814() != 0) {
            D_800C4A18 = 7;
            return;
        }
        break;
    case 7:
        func_800132F0(0x1E, 0, 2);
        ovl_27_func_800BA4C4();
        D_800C4A18 = 8;
        return;
    case 8:
        func_8001FD10();
        if (func_8001FD74() != 0) {
            func_8001FE00(0xA);
            D_800C4A18 = 9;
            return;
        }
        break;
    case 9:
        if (func_8001FE6C() == 0) {
            D_800C4A18 = 0xC;
            return;
        }
        break;
    case 12:
        temp_v0_2 = func_80013394();
        if (temp_v0_2 == 1) {
            D_800C4A18 = 0;
            func_80020818();
            func_8001FB30(0, 0, 0x1A, 0x7F);
            D_8005E5E8->unk19 = 0;
            D_8005E5E8->unk1A = 0;
            D_8005E5E8->unk1B = 0;
            second = &D_8005E5E8[1];
            second->unk19 = 0;
            second->unk1A = 0;
            second->unk1B = 0;
            D_8005E5E8->unk18 = (u8) temp_v0_2;
            D_8005E5E8[1].unk18 = (u8) temp_v0_2;
            state = (Ovl27StateCounterView *)D_8006C838;
            state->counter++;
        }
        break;
    }
}
