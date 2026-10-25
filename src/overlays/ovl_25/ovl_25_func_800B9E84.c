#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_commands.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_25_func_800BA5BC(SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B9E84(void) {
    SVECTOR coords;
    ObjectState *temp_s0;
    ObjectState *temp_s0_2;
    ObjectState *temp_s0_3;
    ObjectState *temp_s0_4;
    u8 temp_v1;

    SelectSpriteAnimation((ObjectState *) &D_800C0394, 0);
    ovl_25_func_800BA5BC(&D_800C0394, (u16 *) D_800BCD28);
    temp_s0 = (ObjectState *) &D_800C0394 + 6;
    SelectSpriteAnimation(temp_s0, 0);
    ovl_25_func_800BA5BC((SpriteSourceData *) temp_s0, (u16 *) D_800BCD30);
    switch (D_800BCD22) {
    case 0:
        temp_s0_2 = (ObjectState *) &D_800C0394 + 0xC;
        SelectSpriteAnimation(temp_s0_2, 0);
        ovl_25_func_800BA5BC((SpriteSourceData *) temp_s0_2, (u16 *) D_800BCD38);
        if (*(&D_800BCD21 + 2) >= 0x1E) {
            D_800BCD22 = 1;
        }
        break;
    case 1:
        temp_s0_4 = (ObjectState *) &D_800C0394 + 0xC;
        if (*(u8 *) ((u8 *) &D_800C0394 + 0x64) != 1) {
            SelectSpriteAnimation(temp_s0_4, 1);
        }
        ovl_25_func_800BA5BC((SpriteSourceData *) temp_s0_4, (u16 *) D_800BCD38);
        D_800BCD38[0] += 0x12;
        if (D_800BCD38[0] >= 0xDBA) {
            D_800BCD22 = 0xFF;
        }
        break;
    case 0xFF:
        temp_s0_3 = (ObjectState *) &D_800C0394 + 0xC;
        SelectSpriteAnimation(temp_s0_3, 0);
        ovl_25_func_800BA5BC((SpriteSourceData *) temp_s0_3, (u16 *) D_800BCD38);
        break;
    }
    temp_v1 = *(&D_800BCD21 + 2);
    switch (temp_v1) {
    case 0:
        SelectSpriteAnimation((ObjectState *) &D_800C0424, 0);
        ovl_25_func_800BA5BC(&D_800C0424, (u16 *) D_800BCD40);
        if (D_800BFE64 >= 0x690) {
            *(&D_800BCD21 + 2) = 1;
        }
        break;
    case 0xFF:
        SelectSpriteAnimation((ObjectState *) &D_800C0424, 0);
        ovl_25_func_800BA5BC(&D_800C0424, (u16 *) D_800BCD40);
        break;
    default:
    {
        ObjectState *base = (ObjectState *) &Ovl25Sprite800C0334;
        *(&D_800BCD21 + 2) += 1;
        if (*(u8 *) ((u8 *) base + 0xF4) != 1) {
            SelectSpriteAnimation(base + 0x1E, 1);
        }
        ovl_25_func_800BA5BC((SpriteSourceData *) (base + 0x1E), (u16 *) D_800BCD40);
        D_800BCD40[0] += 0x12;
        if (D_800BCD40[0] >= 0xFA6) {
            *(&D_800BCD21 + 2) = 0xFF;
        }
        break;
    }
    }
    switch (D_800BCD21) {
    case 0:
        D_800BCD54 = 0x3C;
        break;
    case 1:
        SelectSpriteAnimation((ObjectState *) &Ovl25Sprite800C0334, 0);
        ovl_25_func_800BA5BC(&Ovl25Sprite800C0334, (u16 *) D_800BCD48);
        D_800BCD48[1] -= D_800BCD54;
        D_800BCD54 -= D_800BCD54 / 3;
        if (D_800BCD54 < 6) {
            D_800BCD21 = 2;
        }
        break;
    case 2:
        SelectSpriteAnimation((ObjectState *) &Ovl25Sprite800C0334, 1);
        ovl_25_func_800BA5BC(&Ovl25Sprite800C0334, (u16 *) D_800BCD48);
        D_800BCD48[1] += D_800BCD54;
        D_800BCD54 += (D_800BCD54 * 6) / 5;
        if (D_800BCD48[1] >= 0) {
            D_800BCD21 = 0;
            D_800BCD24 = 1;
            PlaySoundEffect(0x4D);
            SelectSpriteAnimation((ObjectState *) D_800C021C, 0x17);
        }
        break;
    }
    if (D_800BCD24 != 0) {
        coords = *(SVECTOR *) D_800BCD48;
        coords.vx += 0x96;
        coords.vy += 0x64;
        coords.vz += 0x96;
        ovl_25_func_800BA5BC((SpriteSourceData *) D_800C021C, (u16 *) D_800BCD48);
        if (((SpriteSourceData *) D_800C021C)->field_2 & 0x100) {
            D_800BCD24 = 0;
        }
    }
}
