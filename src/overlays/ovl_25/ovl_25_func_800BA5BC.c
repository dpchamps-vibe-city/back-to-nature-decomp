#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_driver.h"
#include "sound_commands.h"


s32 func_8001DFD4 (s32 *arg0, SVECTOR *arg1);
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3);
void func_800248E8 (s32 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_80015F80 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);
void func_800158E4 (SpriteSourceData *src);
void func_80024A4C (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);
void func_80015840(ObjectState *obj, u8 arg1);
void ovl_25_func_800BA758 (SpriteSourceData *arg0, u16 *arg1);

extern s32 D_800BCD58;
extern SVECTOR D_800BCDC4;
extern s16 D_800BFE44;
extern ObjectState D_800C0334;

void ovl_25_func_800BA5BC(SpriteSourceData *arg0, u16 *arg1) {
    s32 temp_s2;

    D_800BCDC4.vx = arg1[0] - 0x96;
    D_800BCDC4.vy = arg1[1] - 0x64;
    D_800BCDC4.vz = arg1[2] - 0x96;
    D_800BCDC4.pad = arg1[3];
    temp_s2 = gActiveRenderContext->field_120 + ((func_8001DFD4(&D_800BCD58, &D_800BCDC4) >> 2) * 4);
    func_80015BF0(temp_s2, arg0, *(s16 *) ((u8 *) &D_800BCD58 + 0), *(s16 *) ((u8 *) &D_800BCD58 + 4));
    if (D_800BFE44 == 0) {
        if ((arg0 == ((&D_800BFE44 + 0x14))) || (arg0 == ((&D_800BFE44 + 0x50))) || (arg0 == ((&D_800BFE44 + 0x8C))) || (arg0 == ((&D_800BFE44 + 0xC8))) || (arg0 == ((&D_800BFE44 + 0x104))) || (arg0 == ((&D_800BFE44 + 0x140))) || (arg0 == ((&D_800BFE44 + 0x17C))) || (arg0 == ((&D_800BFE44 + 0x1B8)))) {
            func_800248E8(temp_s2, *(s16 *) ((u8 *) &D_800BCD58 + 0), *(s16 *) ((u8 *) &D_800BCD58 + 4), 0);
        }
    } else if ((arg0 == ((&D_800BFE44 + 0x17C))) || (arg0 == ((&D_800BFE44 + 0x1B8)))) {
        func_800248E8(temp_s2, *(s16 *) ((u8 *) &D_800BCD58 + 0), *(s16 *) ((u8 *) &D_800BCD58 + 4), 0);
    }
    if ((arg0 == &D_800C0334) || (arg0 == ((&D_800C0334 + 6))) || (arg0 == ((&D_800C0334 + 0xC))) || (arg0 == ((&D_800C0334 + 0x12))) || (arg0 == ((&D_800C0334 + 0x18))) || (arg0 == ((&D_800C0334 + 0x1E)))) {
        func_800248E8(temp_s2, *(s16 *) ((u8 *) &D_800BCD58 + 0), *(s16 *) ((u8 *) &D_800BCD58 + 4), 0);
    }
}
