#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

/* Scan the ten 0x30-byte D_80129230 records for a free slot (field +0x14
 * == 0) and, on the first free one, initialise it from the sprite-data
 * header, hand it to the display helper, and install arg1 at +2 of the
 * 0x18-stride side table pointed to by D_80074838[0x5DD0]. */
void ovl_11_func_800E516C(SpriteDataHeader *arg0, s32 arg1) {
    Ovl11E5230Entry *var_a2;
    Ovl11E5230Entry *var_s0;
    u8 *var_s2;
    s32 var_a0;
    s32 var_a1;
    s32 var_s1;

    var_a2 = D_80129230;
    var_a1 = 0;
    var_s2 = D_80074838;
    var_s1 = 0;
    var_s0 = var_a2;
    var_a0 = 0;
loop_1:
    var_a1 += 1;
    if (((Ovl11E5230Entry *) ((u8 *) var_a2 + var_a0))->unk14 == 0) {
        InitSpriteSourceData(var_s0, arg0);
        SetSpriteSourceTextureOffsets((Struct_800154CC *) var_s0, D_80129410, D_80129412,
                      D_80129410, (s16) (s32) D_80129412);
        *(s16 *) ((u8 *) (var_s1 + *(s32 *) (var_s2 + 0x5DD0)) + 2) = arg1;
        return;
    }
    var_s1 += 0x18;
    var_s0 += 1;
    var_a0 += 0x30;
    if (var_a1 >= 0xA) {
        return;
    }
    goto loop_1;
}
