#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

typedef struct {
    /* 0x00 */ char pad_00[0x16];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ char pad_18[0xA8 - 0x18];
    /* 0xA8 */ UnkStruct80107DE0 unkA8;
} Struct_800D049C;

void ovl_11_func_800D049C(Struct_800D049C *arg0) {
    if (arg0->unk16 >= 10) {
        Ovl11StartTimedSpriteEffect(&arg0->unkA8, 0x25, 0x2D);
    }
}
