#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80015840(ObjectState *obj, u8 arg1);
Ovl11CBF70Effect *ovl_11_func_800CBF70(Ovl11CBF70Position *arg0, u16 *arg1, s16 arg2);

s32 ovl_11_func_800CDD7C(Ovl11C7010Object *arg0) {
    u16 effect[3];

    arg0->unk6C |= 0x1870;
    ovl_11_func_800C70CC(arg0, 0x45, 0, -2);
    func_80015840((ObjectState *) &arg0->sprite, *(u8 *) ((u8 *) arg0 + 0x38));
    PlaySoundEffect(0xF);
    effect[0] = 0x186;
    effect[1] = 0;
    effect[2] = 1;
    ovl_11_func_800CBF70((Ovl11CBF70Position *) ((u8 *) arg0 + 0x100), effect, (s16) arg0->unk38);
    D_8012DB78.x = 0;
    D_8012DB78.y = 0;
    D_8012DB78.z = 0;
    D_8012DB84 = 0x1B;
    if (arg0->unk84 == 0x39) {
        D_8012DB84 = 0x1C;
    }
    D_8012DB88 = 0;
    D_8012DB80 = 0;
    return 1;
}
