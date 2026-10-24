#include "common.h"
#include "game_types.h"

void ovl_11_func_800D7328(s16 *arg0);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800CB6C8(u16 *arg0) {
    Ovl11C7010Object *obj;
    u16 *position;

    obj = (Ovl11C7010Object *) &D_80071A00;
    position = (u16 *) ((u8 *) obj + 0xF0);
    ovl_11_func_800D7328((s16 *) position);
    ovl_11_func_800D7348(position, arg0);
    ovl_11_func_800C70CC(obj, 0x30U, obj->unk38 + 0x21, -2);
}
