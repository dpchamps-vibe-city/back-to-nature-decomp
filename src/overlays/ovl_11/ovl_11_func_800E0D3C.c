#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

s32 ovl_11_func_800E109C(UnkStruct800E109C *arg0);
s16 ovl_11_func_800E0E38(UnkStruct800E109C *arg0);

s32 ovl_11_func_800E0D3C(UnkStruct800E109C *arg0, s32 arg1) {
    char *base;
    char *far_base;
    s32 flags;

    if (ovl_11_func_800E109C(arg0) != 0) {
        return -1;
    }
    memset(arg0, 0, 0xB8);
    if (arg1 == 0x165) {
        *(s16 *)((u8 *)arg0 + 0xB0) = 0xE;
    }
    *(s16 *)((u8 *)arg0 + 0x16) = 0xA;
    arg0->unk0 = (u16)arg1;
    *(s16 *)((u8 *)arg0 + 0xB4) = 7;
    base = (char *)D_8006C838;
    *(Ovl11InitStamp *)((u8 *)arg0 + 0x1A) = *(Ovl11InitStamp *)(base + 0x44B8);
    flags = *(s32 *)((u8 *)arg0 + 0x34);
    *(u16 *)((u8 *)arg0 + 4) = 0xFFFF;
    *(s16 *)((u8 *)arg0 + 0x1A) = 0;
    flags |= 0x10000;
    *(s32 *)((u8 *)arg0 + 0x34) = flags;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == 1 || *(s16 *)(far_base + 0x25476) == 5) {
        ovl_11_func_800E0E38(arg0);
    } else {
        *(s32 *)((u8 *)arg0 + 0x8C) = 0;
    }
    Ovl11ResetTimedSpriteEffect((s16 *)((u8 *)arg0 + 0xA8));
    return 0;
}
