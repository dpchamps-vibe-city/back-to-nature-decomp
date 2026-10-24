#include "common.h"
#include "game_types.h"

s16 ovl_11_func_800F4240(Ovl11ItemCollisionObject *arg0);
s32 ovl_11_func_800D5D38(u16 a, u16 b);
/* Pass the four coordinate payloads as words, as 8012135C does for 80121500. */
s32 ovl_11_func_8012135C();

s32 ovl_11_func_800F46A4(Ovl11SpawnEffectRecord *arg0) {
    s32 result;
    u16 id;
    char *far_base;

    result = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) arg0);
    if (!id) {
        return 0;
    }
    far_base = (char *) &D_8007AFF0;
    if (*(s16 *) (far_base + 0x25474) == 0x13) {
        return 0;
    }
    if (ovl_11_func_800D5D38((u16) arg0->unk2, 0) == 1) {
        arg0->unk4 &= 0xFFFE;
        ovl_11_func_8012135C(arg0->unk2, arg0->unk8, arg0->unkC, arg0->unk10, arg0->unk14);
        result = 1;
    }
    return result;
}
