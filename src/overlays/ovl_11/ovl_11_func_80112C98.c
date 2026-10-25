#include "common.h"
#include "game_flags.h"

typedef struct {
    /* 0x00 */ char pad_00[0x4];
    /* 0x04 */ u16 unk04;
} UnkStruct80112C98;

void ovl_11_func_80112C98(UnkStruct80112C98 *arg0) {
    s32 value;
    unsigned char *base;

    if (arg0->unk04 < 0x3A99) {
        if (GetGameFlag(0x2B) == 0) {
            SetGameFlag(0x4D, 1);
            SetGameFlag(0xAF, 1);
            value = (s32)&D_8006C838;
            base = (unsigned char *)(value + 0x8000);
            if (*(u16 *)(base + 0x6772) < 3) {
                *(u16 *)(base + 0x6772) = *(u16 *)(base + 0x6772) + 1;
            }
        }
    }
}
