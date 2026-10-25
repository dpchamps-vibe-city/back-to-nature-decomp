#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_11_func_800E516C(SpriteDataHeader *, s32);

void ovl_11_func_800E4DAC(s32 arg0) {
    s32 sp18;
    SpriteSourceData *temp_s2;
    s16 temp_a2;
    s16 temp_v1_2;
    s32 *var_a1;
    s32 temp_s1;
    s32 temp_v1;
    s32 var_a1_2;
    u8 *temp_a3;
    u8 *world;
    void *var_v0;

    for (sp18 = 2; sp18 < 0x1A; sp18++) {
        temp_a2 = ((s16 *)arg0)[sp18];
        var_a1_2 = 0xFFFF03FF;
        var_a1 = &D_801291C0;
        temp_v1 = sp18 - 2;
        temp_a3 = D_801291B8 + var_a1[temp_v1];
        temp_s1 = temp_a2 & var_a1_2;
        if (temp_a2 == 0x401) {
            InitSpriteSourceData((SpriteSourceData *) D_80071C90, (SpriteDataHeader *) temp_a3);
            SetSpriteSourceTextureOffsets((Struct_800154CC *) D_80071C90, D_80129410, D_80129412, 0, 0);
        } else if (temp_a2 & 0x2000) {
            temp_s2 = (SpriteSourceData *)(D_800762D0 + (temp_s1 * 0x1D4));
            InitSpriteSourceData(temp_s2, (SpriteDataHeader *) temp_a3);
            if (temp_s1 == 0x21) {
                temp_v1_2 = ((struct Ovl11CurrentObjectView *)(D_80071C90 - 0x5458))->current[1];
                temp_s2 = (SpriteSourceData *)D_8006C838;
                if ((temp_v1_2 == 0xB6) || (temp_v1_2 == 0x131)) {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *) (D_800762D0 + 0x3C54), D_80129410, D_80129412, 0, 0);
                } else {
                    SetSpriteSourceTextureOffsets((Struct_800154CC *) (D_800762D0 + 0x3C54), D_80129410, D_80129412, D_80129410, (s16) (s32) D_80129412);
                }
            } else {
                world = (u8 *)temp_s2;
                temp_s2 = (SpriteSourceData *)D_8006C838;
                SetSpriteSourceTextureOffsets((Struct_800154CC *)world, D_80129410, D_80129412, D_80129410, (s16) (s32) D_80129412);
            }
            world = (u8 *)D_8006C838;
            if (((struct Ovl11CurrentObjectView *)world)->current[1] != 0xA1) {
                var_v0 = (void *)(world + (temp_s1 * 0x1D4));
                ((Ovl11EF488FlagView *)var_v0)->flags |= 0x1000;
            }
        } else if (temp_a2 == 0x404) {
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)&D_8007AFF8);
            ((Ovl11EF488FlagView *)(D_80071C90 - 0x5458 + temp_s1 * 0x1D4))->flags |= 0x1000;
        } else if (temp_a2 == 0x405) {
            var_a1_2 = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[2];
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)var_a1_2);
            ((Ovl11EF488FlagView *)(D_80071C90 - 0x5458 + temp_s1 * 0x1D4))->flags |= 0x1000;
        } else if (temp_a2 == 0x406) {
            var_a1_2 = (s32)D_8008F7F8_words + D_80124FCC_spriteOffsets[6];
            InitSpriteSourceData(&D_8007942C, (SpriteDataHeader *)var_a1_2);
            ((Ovl11EF488FlagView *)(D_80071C90 - 0x5458 + temp_s1 * 0x1D4))->flags |= 0x1000;
        } else if (temp_a2 & 0x1000) {
        } else if (temp_a2 & 0x800) {
            ovl_11_func_800E516C((SpriteDataHeader *)temp_a3, temp_s1);
        }
    }
}
