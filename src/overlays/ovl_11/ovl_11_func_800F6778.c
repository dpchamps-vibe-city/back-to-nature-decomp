#include "common.h"
#include "sound_commands.h"

void ovl_11_func_800F6680(void);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);

void ovl_11_func_800F6778(void) {
    ovl_11_func_800F6680();
    ((struct struct_8006C838_view *)&D_8006C838)->field_0C |= 0x20000;
    PlaySoundEffect(3);
    D_80126E40 = 4;
    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x37A);
}
