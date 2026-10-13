#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F4240(Ovl11ItemCollisionObject *arg0);
void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800D5E00(u16 a, u16 b);
s32 ovl_11_func_800D5E64(u16 a, u16 b);
s32 ovl_11_func_800D5ED4(u16 a, u16 b);
void ovl_11_func_800F3898(u16 arg0, u16 arg1);
/* Forward the aligned payload words as in the matched 800F474C caller. */
s32 ovl_11_func_8012135C();

s32 ovl_11_func_800F482C(Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2) {
    s32 flags;
    s32 result;
    u16 id;

    result = 0;
    id = ovl_11_func_800F4240((Ovl11ItemCollisionObject *) arg0);
    if (id == 0) {
        return 0;
    }
    flags = ((struct Ovl11PoolResetView *) D_8006C838)->flags & ~0x20;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags = flags;
    if (id == 0x174) {
        ((struct Ovl11PoolResetView *) D_8006C838)->flags = flags | 0x20;
        ovl_11_func_800F0FB0(5);
        return 0;
    }
    if (ovl_11_func_800D5E00(id, 0) == 1) {
        if ((u32) arg2 >= ovl_11_func_800D5E64(id, 0)) {
            if ((u32) arg1 >= ovl_11_func_800D5ED4(id, 0)) {
                arg0->unk4 &= 0xFFFE;
                switch (id) {
                case 0x3D:
                case 0x172:
                case 0x173:
                case 0x174:
                    ovl_11_func_800F3898(2, 0);
                    break;
                case 0x3E:
                    ovl_11_func_800F3898(1, 0);
                    break;
                default:
                    break;
                }
                ovl_11_func_8012135C(arg0->unk2, arg0->unk8, arg0->unkC, arg0->unk10, arg0->unk14);
                result = 1;
            }
        }
    }
    return result;
}
