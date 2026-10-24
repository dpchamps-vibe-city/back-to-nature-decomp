#include "common.h"

#include "game_types.h"

void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void ovl_11_func_80100B50(u8 arg0, u8 arg1);
s32 func_800226B0(void);
void func_80022738(void);
s32 func_8001FABC(s16 arg0);
void ovl_11_func_80103714(void);
void ovl_11_func_8010021C(void);

void ovl_11_func_801006F8(void) {
    s32 buttons;

    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x3BA);
    ovl_11_func_80100B50((u8)D_801273EC, 1);
    if (func_800226B0() != 0) {
        buttons = ((GfxObj *)D_8005E3A8)->field_8;
        if (buttons & 0x800) {
            func_80022738();
            Ovl11_D_801273DC = ovl_11_func_80103714;
        } else if (buttons & 0x70) {
            func_80022738();
            func_8001FABC(1);
            Ovl11_D_801273DC = ovl_11_func_8010021C;
        }
    }
}
