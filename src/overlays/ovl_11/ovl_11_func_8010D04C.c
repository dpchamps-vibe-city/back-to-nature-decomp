#include "common.h"
#include "psyq/memory.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk1;
} Ovl11UnalignedWord8010D04C;

s32 ovl_11_func_8010D04C(s32 arg0) {
    char *base;
    s32 temp_v0;

    memset((void *)arg0, 0, 0xF8);
    base = (char *)&D_8006C838;
    *(Ovl11UnalignedWord8010D04C *)(arg0 + 0x1A) = *(Ovl11UnalignedWord8010D04C *)(base + 0x44B8);
    *(s16 *)(arg0 + 0x30) = 1;
    *(u16 *)(arg0 + 0x4) = 0xFFFF;
    *(u16 *)(arg0 + 0xBA) = 0xFFFF;
    *(s16 *)(arg0 + 0xBE) = 0;
    *(s32 *)(arg0 + 0x8C) = 0;
    *(s16 *)(arg0 + 0x16) = 0x32;
    *(s32 *)(arg0 + 0x34) |= 0x3C000000;
    Ovl11ResetTimedSpriteEffect((s16 *)(arg0 + 0xA8));
    temp_v0 = Rand(0x12C);
    *(s16 *)(arg0 + 0x2C) = temp_v0;
    return 0;
}
