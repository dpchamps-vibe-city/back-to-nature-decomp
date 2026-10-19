#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

void *ovl_11_func_800BDCE8(u8 *arg0, Ovl11Func800BDCE8Arg1 *arg1, s32 arg2);
s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);

s32 ovl_11_func_800D2DB8(Ovl11Func800BDCE8Arg1 *arg0) {
    if (D_80128D6C == 1) {
        if (ovl_11_func_800BDCE8((u8 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header, arg0, 0) != NULL) {
            return 1;
        }
    }
    return ovl_11_func_800BDB88(&arg0->unk0, 0xC000) != 0;
}
