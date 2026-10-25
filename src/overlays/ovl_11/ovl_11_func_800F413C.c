#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "sprite_rotation.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "random.h"
#include "vblank_timer.h"

typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
} M2C_2bc7a43223fd_CoordTri;

typedef struct {
    char pad0[2];
    s16 unk2;
    u16 unk4;
} M2C_4c1a205b3b09_ReconA0View;

typedef struct {
               char pad0[0x38];
               u16 unk38;
               char pad3A[0x100 - 0x3A];
                s32 unk100;
                s32 unk104;
                s32 unk108;
} M2C_ad72053e5aa9_StructOvl11CE034A;

typedef struct {
               s32 unk0;
               s32 unk4;
               s32 unk8;
} M2C_ad72053e5aa9_StructOvl11CE034B;

typedef struct {
               s16 unk0;
               s16 unk2;
               u16 unk4;
               s16 pad6;
               s32 unk8;
               s32 unkC;
               s32 unk10;
} M2C_701317280b4c_Ov11_4360struct;

typedef struct {
    char pad_00[0x24];
    s16 unk24;
    char pad_26[0x30 - 0x26];
    s16 unk30;
    char pad_32[0x7A - 0x32];
    u16 unk7A;
} M2C_1437d20b8085_Ovl11D04D4Obj;

typedef struct {
    u8 b[4];
} M2C_1d1631a2972a_Ov11_F43CCArgBlk;

void OuterProduct0 (VECTOR *v0, VECTOR *v1, VECTOR *v2);
s32 ovl_11_func_800F581C (void);
s32 func_8001E78C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8001E38C (void);
s32 func_8001E7DC (s32 *arg0, s32 *arg1);
s32 func_8001E878 (M2C_2bc7a43223fd_CoordTri *p0, M2C_2bc7a43223fd_CoordTri *p1, M2C_2bc7a43223fd_CoordTri *p2);
void SetVal8005E2E8 (s32 arg0);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800BD358 (void);
s16 ovl_11_func_800F4240(M2C_4c1a205b3b09_ReconA0View *arg0);
s32 ovl_11_func_800D7504 (s16 arg0);
void ovl_11_func_800CCFF8 ();
void ovl_11_func_800C6E0C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6E8C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
void ovl_11_func_800C6F0C ();
void ovl_11_func_800C6F6C (Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1);
s32 ovl_11_func_800D589C (s16 arg0);
void ovl_11_func_800CE034 (M2C_ad72053e5aa9_StructOvl11CE034A *arg0, M2C_ad72053e5aa9_StructOvl11CE034B *arg1, s32 arg2);
void ovl_11_func_800D666C (s16 arg0);
void ovl_11_func_800D7328 (s16 *arg0);
void ovl_11_func_800BD8DC (s16 arg0);
s32 ovl_11_func_800E6EAC(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
void ovl_11_func_80107DD0 (s16 *arg0);
s32 ovl_11_func_800D3104 (u16 *arg0, s32 arg1);
s32 ovl_11_func_800CF258 (s32 arg0);
void ovl_11_func_800D3200 (s32 arg0);
s32 ovl_11_func_800D2594 (Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1);
s32 ovl_11_func_800D1C18 (s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800D1CD0 (s32 arg0, s32 arg1);
s32 ovl_11_func_800D5750 (s16 arg0);
s32 ovl_11_func_800D5868 (s16 arg0);
s32 ovl_11_func_800D5C3C (s16 arg0);
s32 ovl_11_func_800D5CE4 (s16 arg0);
s32 ovl_11_func_800D5C90 (s16 arg0);
s32 pow_int (s32 arg0, s32 arg1);
s32 func_8001AF44 (u32 arg0);
void ovl_11_func_800F4360 (M2C_701317280b4c_Ov11_4360struct *arg0, s16 arg1, s16 arg2, Vec3 vec);
s32 Ovl11IsActionGuardAreaBlocked (s32 arg0);
s32 Ovl11TestRecordFieldGuard (s32 arg0, s32 arg1);
s32 Ovl11TestSelectorRangeGuard (u32 arg0);
s32 ovl_11_func_800C3548 (s32 arg0);
s32 Ovl11IsOutsideGuardInterval (s32 arg0, s32 arg1);
s32 Ovl11TestAreaFlagGuard (s32 arg0);
s32 Ovl11HasActionGuardAreaException (s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D04D4 (M2C_1437d20b8085_Ovl11D04D4Obj *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);
void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);

typedef struct {
    s16 unk0;
    char pad2[2];
    u16 unk4;
} M2C_4c1a205b3b09_F413CArg1;

typedef struct {
    char pad_00[0x44BA];
    s16 unk_44BA;
} M2C_4c1a205b3b09_F413CStateView;

s32 ovl_11_func_800F413C(M2C_4c1a205b3b09_ReconA0View *arg0, M2C_4c1a205b3b09_F413CArg1 *arg1) {
    s32 temp_v0;
    u16 temp_a0;

    temp_v0 = (ovl_11_func_800F581C() | 0x81) & 0xFFFF;
    temp_a0 = arg0->unk4;
    if (temp_v0 != (temp_a0 & temp_v0)) {
        return 0;
    }
    if ((u16) arg0->unk2 == 0x139) {
        if (temp_a0 & 0x4000) {
            return 0;
        }
        arg0->unk4 = temp_a0 | 0x4000;
    } else {
        arg0->unk4 = temp_a0 & 0xFFFE;
    }
    if (arg1 != NULL) {
        arg1->unk0 = (u16) arg0->unk2;
        arg1->unk4 = 1;
    }
    if (((M2C_4c1a205b3b09_F413CStateView *) D_8006C838)->unk_44BA == 2) {
        if ((s16) arg1->unk0 == 0x40) {
            arg1->unk0 = 0x13AU;
        }
    }
    if (((s16) arg1->unk0 == 0x3A) && (((M2C_4c1a205b3b09_F413CStateView *) D_8006C838)->unk_44BA == 3)) {
        arg1->unk0 = 0x13BU;
    }
    if ((s16) arg1->unk0 == 0x3E) {
        if (((M2C_4c1a205b3b09_F413CStateView *) D_8006C838)->unk_44BA == 3) {
            arg1->unk0 = 0x13CU;
            return 1;
        }
    }
    return 1;
}
