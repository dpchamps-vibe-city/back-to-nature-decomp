#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "cd_loader.h"

s32 ovl_11_func_800D5750 (s16 arg0);
s32 ovl_11_func_800D6730 (s16 value, s16 row);
s32 ovl_11_func_800D688C (s16 arg0);
void func_80015704 ();
s32 ovl_11_func_800D678C (s16 arg0);
s32 ovl_11_func_800D67F4 (s16 arg0);
void ovl_11_func_800D6944 (SpriteSourceData *arg0, u16 *arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6);
void func_80017200 (u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

s32 ovl_11_func_800D6A94(s16 arg0) {
    s16 origin[4];
    SpriteSourceData sp28;
    SpriteSourceData *sprite;
    s32 sp58;
    s16 sp5C;
    s32 sp60;
    s16 temp_v0_4;
    s32 var_fp;
    s32 temp_a0;
    s32 temp_a2;
    s32 tableBase;
    s32 temp_s2;
    s32 temp_v0;
    s32 temp_v0_5;
    s32 var_a0;
    s32 var_s3;
    u16 *temp_a1;
    u16 *var_s1;
    u16 *var_s7;
    u16 *temp_a3;
    Ovl11A04B8Entry *temp_s0;
    SpriteTableRef *temp_v0_2;
    SpriteTableRef *temp_v0_3;
    SpriteTextureRect *var_s4;
    SpriteTextureRect *var_s5;

    var_fp = 0;
    sp58 = 0;
    sp5C = 0;
    if (arg0 == 0) {
        return 0;
    }
        var_s3 = ovl_11_func_800D5750(arg0);
        if (var_s3 == 2) {
            var_s3 = 1;
        }
        temp_s2 = ovl_11_func_800D6730(arg0, (s16) var_s3);
        if (temp_s2 == -1) {
            return 0;
        }
        temp_v0 = ovl_11_func_800D688C(arg0);
        if (temp_v0 == 0) {
            CdReadDiscSectorsBlocking(0, (arg0 << 0xB) + 0x03C19000, 0x800, 0, (s32) &gMessageWindowReadBuffer);
            func_80015704(&sp28, (SpriteDataHeader *) &gMessageWindowReadBuffer);
            sprite = &sp28;
        } else {
            sp58 = 1;
            func_80015704(&sp28, (SpriteDataHeader *) temp_v0);
            sprite = &sp28;
        }
        var_s5 = NULL;
        var_s7 = NULL;
        var_s4 = NULL;
        var_s1 = NULL;
        temp_a3 = (u16 *) sprite->field_28;
        temp_a2 = sprite->field_2C;
        temp_a1 = (u16 *) (temp_a2 + temp_a3[1]);
        temp_s0 = &D_800A04B8[var_s3][temp_s2];
        if ((u16) *temp_a1 <= 0xFFFDU) {
            tableBase = sprite->field_20;
            temp_v0 = tableBase + *temp_a1 * 4;
            temp_v0_2 = (SpriteTableRef *) temp_v0;
            var_fp = temp_v0_2->count;
            tableBase = sprite->field_24 + temp_v0_2->offset;
            var_s7 = (u16 *) (((SpriteFramePiece *) tableBase) + (var_fp - 1));
            var_s5 = &((SpriteTextureRect *) sprite->field_1C)[*var_s7];
        }
        temp_a1 = (u16 *) (temp_a2 + temp_a3[3]);
        if ((u16) *temp_a1 <= 0xFFFDU) {
            tableBase = sprite->field_20;
            temp_v0 = tableBase + *temp_a1 * 4;
            temp_v0_3 = (SpriteTableRef *) temp_v0;
            tableBase = sprite->field_24 + temp_v0_3->offset;
            var_s1 = (u16 *) (((SpriteFramePiece *) tableBase) + ((sp5C = temp_v0_3->count) - 1));
            var_s4 = &((SpriteTextureRect *) sprite->field_1C)[*var_s1];
        }
        temp_s2 = (s16) var_s3;
        temp_v0_5 = ovl_11_func_800D678C((s16) temp_s2);
        temp_s0->unk8 = 0;
        temp_s0->unkA = 0U;
        temp_s0->unk10 = 0U;
        temp_s0->unk12 = 0U;
        temp_s0->unkC = 0;
        temp_s0->unkE = 0U;
        temp_s0->unk14 = 0U;
        temp_s0->unk16 = 0U;
        if (temp_v0_5 != 0) {
            temp_s0->unk18 = 0x3F0;
            temp_s0->unk1A = (s16) (var_s3 + 0xFE);
            temp_s0->unk0 = (u16) ((var_s3 * 8) + 0x28);
            temp_s0->unk2 = 0xBDU;
            origin[0] = 0x280;
        } else {
            temp_s2 = ovl_11_func_800D67F4((s16) temp_s2);
            temp_a0 = temp_s2 / 4;
            temp_s0->unk18 = (s16) (((temp_s2 - (temp_a0 * 4)) * 0x10) + 0x340);
            temp_s0->unk1A = (s16) ((var_s3 * 2) + temp_a0 + 0xFB);
            temp_s0->unk0 = (u16) (temp_s2 * 8);
            temp_s0->unk2 = (u16) ((var_s3 << 5) + 0x9B);
            origin[0] = 0x200;
        }
        origin[1] = 0;
        if (var_s5 != NULL) {
            temp_s0->unk8 = (s16) (var_s5->width * 4);
            temp_s0->unkA = (u16) var_s5->height;
            temp_s0->unk10 = (u16) (*(u16 *) ((u8 *) var_s7 + 4));
            temp_s0->unk12 = (u16) (*(u16 *) ((u8 *) var_s7 + 6));
        }
        if (var_s4 != NULL) {
            temp_s0->unkC = (s16) (var_s4->width * 4);
            temp_s0->unkE = (u16) var_s4->height;
            temp_s0->unk14 = (u16) (*(u16 *) ((u8 *) var_s1 + 4));
            temp_s0->unk16 = (u16) (*(u16 *) ((u8 *) var_s1 + 6));
        }
        if (var_s1 != NULL) {
            temp_s0->unk4 = (u16) temp_s0->unk0;
            if ((var_s5->y == var_s4->y) && (var_s5->x == var_s4->x)) {
                temp_s0->unk6 = (u16) temp_s0->unk2;
            } else {
                temp_s0->unk6 = (u16) (temp_s0->unk2 + temp_s0->unkA);
            }
        }
        if (sp58 != 0) {
            if (var_s7 != NULL) {
                ovl_11_func_800D6944(sprite, var_s7, var_fp, origin[0] + (u16) temp_s0->unk0 + 0x140, origin[1] + (u16) temp_s0->unk2, temp_s0->unk18, temp_s0->unk1A);
            }
            if (var_s1 != NULL) {
                ovl_11_func_800D6944(sprite, var_s1, (s16) sp5C, origin[0] + (u16) temp_s0->unk0 + 0x140, origin[1] + (u16) temp_s0->unk6, temp_s0->unk18, temp_s0->unk1A);
                return 1;
            }
            goto block_32;
        }
        func_80017200(D_8006CA10, origin[0] + (u16) temp_s0->unk0, origin[1] + (u16) temp_s0->unk2, (u16) temp_s0->unk18 - 0x140, (u16) temp_s0->unk1A - 0xFE);
block_32:
        return 1;
}
