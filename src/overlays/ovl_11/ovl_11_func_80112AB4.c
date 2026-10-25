#include "common.h"
#include "game_flags.h"

typedef struct {
    /* 0x00 */ char pad_00[0x4];
    /* 0x04 */ u16 unk04;
} UnkStruct80112C98;

void ovl_11_func_80112AB4(UnkStruct80112C98 *arg0) {
    s32 value;
    unsigned char *base;

    if (GetGameFlag(0x4F) != 1) {
        value = (s32)&D_8006C838;
        base = (unsigned char *)(value + 0x8000);
        if (*(s16 *)(base + 0x6774) < 0x1E) {
            *(u16 *)(base + 0x6774) += 1;
            return;
        }
        if (GetGameFlag(0x4E) == 1) {
            *(u16 *)(base + 0x6770) += 1;
            return;
        }
        if (arg0->unk04 > 0xFDE7U) {
            SetGameFlag(0x4E, 1);
        }
    }
}
