#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void func_800249C0(s32 arg0, s16 arg1, s16 arg2);

s32 ovl_15_func_80137228(s16 arg0, s16 arg1) {
    func_800249C0(gActiveRenderContext->field_D8 + 4, 0x11D, 0xD9);
    if (((GfxObj *)gPad0Input)->field_8 == 0) {
        return arg0;
    } else {
        PlaySoundEffect(0);
        return arg1;
    }
}
