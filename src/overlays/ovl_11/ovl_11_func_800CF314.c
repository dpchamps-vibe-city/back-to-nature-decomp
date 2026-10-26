#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
} Ovl11CF314Arg1;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} Ovl11CF314Arg2;

extern u16 D_80075B0C[];

void ovl_11_func_800CF314(u8 *arg0, Ovl11CF314Arg1 *arg1, Ovl11CF314Arg2 *arg2) {
    ((s32 *)D_80075B0C)[0] = arg2->unk0;
    ((s32 *)D_80075B0C)[1] = arg2->unk4;
    ((s32 *)D_80075B0C)[2] = arg2->unk8;
    D_80075B0C[-4] = arg1->unk2;
    D_80075B0C[-5] = arg1->unk0;
    Ovl11ResetTimedSpriteEffect((s16 *)(arg0 + 0xA8));
}
