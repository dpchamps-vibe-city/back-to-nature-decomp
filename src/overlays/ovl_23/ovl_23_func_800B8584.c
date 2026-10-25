#include "common.h"
#include "sound_commands.h"

void ovl_23_func_800B9310(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_23_func_800B8584(void) {
    ovl_23_func_800B9310();
    func_8002261C(4, 0x2B);
    if (GetVal8005E5B4() == 2) {
        if (GetVal8005E5B8() == 1) {
            Ovl23_D_800BF87C_header.unk414 = PlaySoundEffect(0x41);
            Ovl23_D_800BF87C_header.unk0 = 3;
        } else {
            Ovl23_D_800BF87C_header.unk0 = 1;
        }
    }
}
