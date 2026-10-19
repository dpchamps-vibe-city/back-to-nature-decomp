#include "common.h"
#include "game_types.h"

/* Overlay name for the matched s32 GetVal8005E5B4(void) at 0x800226A4. */
s32 func_800226A4(void);
s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
void ovl_11_func_80110E34(Ovl11MotionState *arg0);

void ovl_11_func_8010CF58(Ovl11CF58Object *arg0) {
    if (arg0->unkB8 & 0x800) {
        if (func_800226A4() == 2) {
            arg0->unkB8 &= ~0x800;
            ovl_11_func_800E6EE0(Ovl11Item_80071A8A.id, 1, 0, 1);
            if (func_8001AF44(2) == 0 && arg0->unk30 == 0x28 && arg0->unk26 != 6) {
                ovl_11_func_80110E34((Ovl11MotionState *)arg0);
            }
        }
    } else if (arg0->unk30 == 0x28 && func_800226A4() == 2 && func_8001AF44(2) == 0 && arg0->unk26 != 6) {
        ovl_11_func_80110E34((Ovl11MotionState *)arg0);
    }
}
