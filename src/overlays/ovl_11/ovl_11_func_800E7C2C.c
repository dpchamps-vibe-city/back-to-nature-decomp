#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

extern s16 D_80071CCC[];
extern s16 D_80076300[];

typedef struct {
    s16 unk0;
    s16 unk2;
} UnkStruct80107DE0;

s32 ovl_11_func_800E7C2C(s16 arg0, s16 arg1) {
    s16 *p;

    if (arg0 == 0x29) {
        p = D_80071CCC;
    } else {
        p = (s16 *)((char *)D_80076300 + arg0 * 0x1D4);
    }
    if (p[0] != -1) {
        if (p[1] == 0) {
            Ovl11ResetTimedSpriteEffect(p);
            return 1;
        }
        return 0;
    }
    Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)p, arg1, 0x1E);
    return 0;
}
