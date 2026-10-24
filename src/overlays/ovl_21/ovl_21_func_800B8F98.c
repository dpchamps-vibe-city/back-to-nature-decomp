#include "common.h"
#include "game_types.h"
#include "psyq/libspu.h"

void ovl_21_func_800BA300(void);
void ovl_21_func_800B990C(void);
void ovl_21_func_800BA4C0(void);
s32 ovl_21_func_800B9844(void);
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void func_80022738(void);

void ovl_21_func_800B8F98(void) {
    s32 i;
    u8 *base;
    u8 *p;

    ovl_21_func_800BA300();
    ovl_21_func_800B990C();
    for (i = 0; i < 6; i++) {
        if (((UnkStruct800C0448 *)D_800C0448)[i].unk18 == 7) {
            ((UnkStruct800C0448 *)D_800C0448)[i].unk2E++;
            if (((UnkStruct800C0448 *)D_800C0448)[i].unk2E >= 30) {
                ((UnkStruct800C0448 *)D_800C0448)[i].unk2E = 0;
                ((UnkStruct800C0448 *)D_800C0448)[i].unk18 = 9;
            }
        }
    }
    ovl_21_func_800BA4C0();
    base = (u8 *)D_800C0448;
    (*(s16 *)(base + 0x988))++;
    if (SpuGetKeyStatus(D_800BCC60[*(s16 *)(base + 0x986)]) == 0 ||
        *(s16 *)(base + 0x988) >= 150) {
        SetVal8005E2BC(0);
        SetVal8005E334(0);
        func_8002261C(4, 0x1D);
    }
    if (ovl_21_func_800B9844() == 0 && GetVal8005E5B4() == 5) {
        func_80022738();
        p = (u8 *)D_800C0448;
        *(s16 *)(p + 0x988) = 0;
        D_800C0448[0] = 9;
    }
}
