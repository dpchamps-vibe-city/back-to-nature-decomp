#include "common.h"
#include "game_types.h"
#include "ovl11_sprite_effect_pool.h"

s16 ovl_11_func_800F4240(Ovl11ItemCollisionObject *arg0);
s32 ovl_11_func_800D5D9C(u16 a, u16 b);
s32 ovl_11_func_800D5E64(u16 a, u16 b);
s32 ovl_11_func_800D5ED4(u16 a, u16 b);

s32 ovl_11_func_800F474C(Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2) {
    s32 result;
    u16 id;

    result = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) arg0);
    if (id == 0) {
        return 0;
    }
    if (ovl_11_func_800D5D9C(id, 0) == 1) {
        if ((u32) arg2 >= ovl_11_func_800D5E64(id, 0)) {
            if ((u32) arg1 >= ovl_11_func_800D5ED4(id, 0)) {
                arg0->unk4 &= 0xFFFE;
                Ovl11SpawnSpriteEffect(arg0->unk2, *(VECTOR *)&arg0->unk8);
                result = 1;
            }
        }
    }
    return result;
}
