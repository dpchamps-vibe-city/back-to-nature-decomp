#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800E0AFC(Recon_ovl_11_func_800E0AFC_A0View *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80015868(Struct_800154CC *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

s16 ovl_11_func_800E0E38(Recon_ovl_11_func_800E0AFC_A0View *arg0) {
    SpriteSourceData *temp_s0;
    s32 temp_v0;
    s32 var_a2;

    temp_v0 = ovl_11_func_800E0AFC(arg0);
    var_a2 = 5;
    switch (temp_v0) {                              /* switch 1; irregular */
    case 0x164:                                     /* switch 1 */
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        break;
    case 0x165:                                     /* switch 1 */
        (*(s16 *) ((u8 *) arg0 + 0)) = (s16) temp_v0;
        var_a2 = 6;
        break;
    }
    do {
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & 0xFBFFFFFF);
        if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
            var_a2 = 8;
        }
    } while (0);
    do {
        do {
            do {
                temp_s0 = (SpriteSourceData *)((u8 *)arg0 + 0x78);
            } while (0);
        } while (0);
    } while (0);
    if ((u16) (*(u16 *) ((u8 *) arg0 + 0xB4)) < 7U) {
        var_a2 = 9;
    }
    func_80015704((SpriteSourceData *) temp_s0, (SpriteDataHeader *) ((u8 *) D_8008F7F8_words + D_80124FCC_spriteOffsets[var_a2]));
    if ((*(u16 *) ((u8 *) arg0 + 0xAE)) != 0) {
        switch (temp_v0) {                          /* switch 2; irregular */
        case 0x164:                                 /* switch 2 */
            func_80015868((Struct_800154CC *) temp_s0, 0, 0, 0, 3);
            break;
        case 0x165:                                 /* switch 2 */
            func_80015868((Struct_800154CC *) temp_s0, 0, 0, 0, 2);
            break;
        }
        if ((*(u16 *) ((u8 *) arg0 + 0xB2)) != 0) {
            func_80015868((Struct_800154CC *) temp_s0, 0, 0, 0x20, 0);
        }
    }
    return 0;
}
