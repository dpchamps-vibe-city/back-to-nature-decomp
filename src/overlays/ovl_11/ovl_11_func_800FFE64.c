#include "common.h"
#include "sound_commands.h"

extern s32 D_801273DC;
extern s8 D_801273E4;
extern s8 D_801273E5;

void ovl_11_func_800FFDCC(void);
void ovl_11_func_800F69D0(void);
void ovl_11_func_80100128(void);
s32 ovl_11_func_800F77A8(s16 arg0);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);

void ovl_11_func_800FFE64(void) {
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    ovl_11_func_800FFDCC();
    ovl_11_func_800F69D0();
    D_801273E4 = 1;
    D_801273E5 = 0;
    D_801273DC = (s32)ovl_11_func_80100128;
    PlaySoundEffect(3);
    ovl_11_func_800F77A8(-1);
    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x3B5);
}
