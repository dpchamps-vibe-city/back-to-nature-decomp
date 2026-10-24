#include "common.h"
#include "game_types.h"

s32 GetVal8005E524(void);

s32 ovl_11_func_800BF3D0(void) {
    Recon_ovl_11_func_800BF3D0_CallRet1View * callRet1;
    callRet1 = GetVal8005E524();
    return ((u32)callRet1->unk0) >> 7;
}
