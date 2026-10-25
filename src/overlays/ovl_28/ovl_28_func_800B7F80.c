#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"


s32 ovl_28_func_800B8B70 (void);
void func_8001719C (u8 *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80015840(ObjectState *obj, u8 arg1);

void ovl_28_func_800B7F80(void) {
    s32 *base;

    if ((ovl_28_func_800B8B70() != 0) && (IsScreenFadeComplete() == 1)) {
        D_800B961C = 0;
        base = (s32 *)&D_8006C838;
        base[0x1122] = base[0x1122] + 1;
    }
}
