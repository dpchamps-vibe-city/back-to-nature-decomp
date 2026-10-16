#include "common.h"

s32 ovl_11_func_800D6A94(s16 arg0);

void ovl_11_func_80102B6C(u16 arg0, s16 arg1) {
    s32 flags;

    ovl_11_func_800D6A94(arg0);
    ((struct Ovl11SpriteRequestState *) D_8006C838)->id = arg0;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->value = arg1;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->active = 1;
    flags = ((struct Ovl11SpriteRequestState *) D_8006C838)->flags;
    flags |= 0x2000;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->flags = flags;
}
