#include "common.h"
#include "game_types.h"

void func_80015840(ObjectState *obj, u8 arg1);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);

void ovl_19_func_800BAD50(void) {
    u8 *base;

    func_80015840((ObjectState *)&gOvl19UiSprite, 9);
    base = gOvl19UiSprite - 0x1A0;
    func_80015EE8(gActiveRenderContext->field_D8 + 4, (s32)&gOvl19UiSprite, base[0x1A4],
                  base[0x1A5], 0, 0);
}
