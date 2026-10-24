#include "common.h"
#include "game_types.h"

u32 Rand(s32 arg0);
void ovl_11_func_800D3200(s32 arg0);
s32 ovl_11_func_800D04D4(void *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);
s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);
void ovl_11_func_800D0408(u16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_800D4314(Ovl11D4314Object *arg0) {
    s16 state;
    s32 result;

    state = arg0->state;
    result = 0;
    switch (state) {
    case 0:
        arg0->state = 1;
        arg0->flags |= 0x800;
        arg0->timer = Rand(60);
        break;
    case -1:
        ovl_11_func_800D3200((s32)arg0);
        result = 1;
        break;
    case 1:
    default:
        if (ovl_11_func_800D04D4(arg0, &D_80123BD4, &arg0->state, -1, &arg0->timer)) {
            arg0->state = -1;
        } else {
            switch (arg0->state) {
            case 1:
            case 2:
                if (ovl_11_func_800BE208(arg0->position, arg0->collision, 0, 0, 0, 0, 0, 1)) {
                    arg0->state = 4;
                    do {
                        arg0->direction = Rand(4);
                    } while (arg0->direction == 1);
                }
                break;
            case 3:
                if (Rand(100) < 30 && !ovl_11_func_800BE208(arg0->position, arg0->collision, 0, 0, 0, 0, 0, 1)) {
                    arg0->state = 1;
                    ovl_11_func_800D04D4(arg0, &D_80123BD4, &arg0->state, -1, &arg0->timer);
                } else {
                    arg0->state++;
                    do {
                        arg0->direction = Rand(4);
                    } while (arg0->direction == 1);
                }
                break;
            case 4:
                ovl_11_func_800D0408(arg0->direction, &arg0->velocity, 60);
                arg0->velocity.unk4 -= 16;
                break;
            case 5:
                ovl_11_func_800D0408(arg0->direction, &arg0->velocity, 60);
                break;
            }
        }
        break;
    }
    return result;
}
