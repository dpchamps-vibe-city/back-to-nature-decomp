#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_projected_record_sprites.h"

void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5,
                   s16 arg6, s32 arg7, s32 arg8, u16 arg9);

void ovl_11_func_800CE0F0(Ovl11SpritePositionView *position, u8 *object,
                          s32 mode, u32 index) {
    SpriteSourceData *source;
    s32 state;
    s32 animation;
    s32 frame;
    u32 work;
    u16 input[4];
    s32 projected[4];

    if (mode != 0) {
        source = &D_80128D00;
        state = index * 4;
        /* Read the indexed state through its halfword and word strides. */
        animation = *(s16 *)(object - (state >> 1) * -2 + 0x3D4);
        frame = *(s16 *)(object - (state >> 2) * -4 + 0x3D6);
    } else {
        source = (SpriteSourceData *)(object + 0x2D4);
        animation = source->field_4;
        state = source->field_5;
        frame = (s16)state;
    }
    work = (u32)input;
    input[0] = position->field_0 + 0x96;
    input[1] = position->field_4 - 0x64;
    input[2] = position->field_8 - 0x96;
    if (Ovl11ProjectRecordSprite((u16 *)work, projected) != 0) {
        func_80016054(gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4),
                      gActiveRenderContext->field_118, (s32)source,
                      (s32)(animation & 0xFF), (u8)(frame & 0xFF),
                      (s16)projected[0], (s16)projected[1], source->field_8,
                      0, 0x40);
    }
    if (mode == 0) {
        work = 0x08000000;
        if (!(D_8006C844 & work)) {
            UpdateSpriteAnimation(source);
        }
    }
}
