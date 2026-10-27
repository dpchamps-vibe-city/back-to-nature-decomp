#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_cd_audio_request.h"

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ char pad_2[0x2E];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ char pad_32[0x2];
    /* 0x34 */ s32 unk34;
} UnkStruct8010BB18;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
} UnkStruct80107DE0;

s32 ovl_11_func_8010BB18(UnkStruct8010BB18 *arg0) {
    char *far_base;

    if (arg0->unk0 == 0) {
        return -1;
    }
    if (!(arg0->unk34 & 4)) {
        Ovl11StartTimedSpriteEffect((UnkStruct80107DE0 *)((u8 *)arg0 + 0xA8), 0x25, 0x2D);
    }
    arg0->unk34 |= 4;
    far_base = (char *)&D_8007AFF0;
    if (*(s16 *)(far_base + 0x25476) == arg0->unk30) {
        Ovl11RequestCdAudioTrack(0xD);
    }
    return -1;
}
