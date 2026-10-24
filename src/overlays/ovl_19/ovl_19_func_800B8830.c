#include "common.h"
#include "game_types.h"

void ovl_19_func_800B843C(void);
void ovl_19_func_800BA770(void);
/* Executable aliases: SetVal8005E2BC, SetVal8005E334 and GetVal8005E5B4. */
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void ovl_19_func_800BAC50(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2);
void func_80022738(void);

void ovl_19_func_800B8830(void) {
    ovl_19_func_800B843C();
    ovl_19_func_800BA770();
    SetVal8005E2BC(0);
    SetVal8005E334(0);
    func_8002261C(4, Ovl19_D_800BCFF0[D_800BF4C0[4]]);
    if (GetVal8005E5B4() == 5) {
        D_800BF4C0[0x100]++;
        if (D_800BF4C0[0x100] >= 60) {
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x50], 1, 0);
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x70], 1, 0);
            ovl_19_func_800BAC50((Ovl19Func800BAC40Arg *)&D_800BF4C0[0x90], 4, 0);
            func_80022738();
            D_800BF4C0[0x100] = 0;
            D_800BF4C0[2] = 4;
        }
    }
}
