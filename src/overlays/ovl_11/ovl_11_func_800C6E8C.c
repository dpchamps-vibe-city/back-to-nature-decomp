#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_11_func_800C6E8C(Recon_ovl_11_func_800C6F0C_A0View *arg0, Recon_ovl_11_func_800C6F0C_A1View *arg1) {
    switch (arg0->unk3C) {
    case 1:
        arg1->unk0.unk0_w = 0x29;
        break;
    case 2:
        arg1->unk0.unk0_w = arg0->unk38 + 0x2A;
        break;
    case 4:
        arg1->unk0.unk0_w = arg0->unk38 + 0x13;
        break;
    }
    SelectSpriteAnimation((ObjectState *)((u8 *)arg0 + 0x260), arg1->unk0.unk0_b);
}
