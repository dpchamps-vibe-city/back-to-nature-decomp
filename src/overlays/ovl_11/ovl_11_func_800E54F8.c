#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libapi.h"
#include "psyq/memory.h"
#include "ovl11_sprite_block_bank.h"

u8 *ovl_11_func_800E54F8(u16 *arg0, s32 arg1) {
    arg1 -= 0x15F;
    return Ovl11ReadSpriteBlock(arg0, gOvl11SpriteBankVramXOffset, 0, arg1);
}
