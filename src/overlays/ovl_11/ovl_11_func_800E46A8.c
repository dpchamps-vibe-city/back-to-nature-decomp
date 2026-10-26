#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "game_flags.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_projected_record_sprites.h"
#include "ovl11_surface_height.h"
#include "ovl11_shared_animation_icon.h"

/* Engine update-latch setter, exe:0x800158D8. */
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                  s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);

void ovl_11_func_800E46A8(u8 *arg0) {
    s32 screen[4];
    u16 box[3];
    s32 ot;
    s32 paused;

    paused = ((Ovl11E46A8State *)D_8006C838)->flags & 0x08000000;
    if (!paused) {
        if (*(s32 *)(arg0 + 0x70) || *(s32 *)(arg0 + 0x74) ||
            *(s32 *)(arg0 + 0x78)) {
            *(s32 *)(arg0 + 0x60) += *(s32 *)(arg0 + 0x70);
            *(s32 *)(arg0 + 0x64) += *(s32 *)(arg0 + 0x74);
            *(s32 *)(arg0 + 0x68) += *(s32 *)(arg0 + 0x78);
            Ovl11ResolveSurfaceHeight((s32 *)(arg0 + 0x60), 0xC000);
        }
    }
    box[0] = *(u16 *)(arg0 + 0x60) + 0x96;
    box[1] = *(u16 *)(arg0 + 0x64) - 0x64;
    box[2] = *(u16 *)(arg0 + 0x68) - 0x96;
    if (Ovl11ProjectRecordSprite(box, screen)) {
        ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
        if (GetGameFlag(0x72) && *(s16 *)arg0 == ((Ovl11E46A8State *)D_8006C838)->selectedId) {
            ot -= 0x14;
        }
        SetSpriteAnimationUpdateLatch(0);
        func_80016054(ot, gActiveRenderContext->field_118, (s32)(arg0 + 0xB0),
                      arg0[0xB4], arg0[0xB5], (s16)screen[0], (s16)screen[1],
                      *(s32 *)(arg0 + 0xB8), 0, 0x40);
        Ovl11DrawTimedSpriteEffect(arg0 + 0xE0, ot, (s16)screen[0],
                              (s16)(screen[1] - 0x2D));
        if (!(*(u16 *)(arg0 + 0x20) & 0x8000)) {
            Ovl11DrawSharedAnimationIcon(ot, screen[0], screen[1],
                                 (*(u16 *)(arg0 + 0x20) >> 14) & 1);
        }
    }
    if (!paused || ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x19D ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BC ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BD ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BE ||
        ((Ovl11E46A8State *)D_8006C838)->current[1] == 0x1BF) {
        UpdateSpriteAnimation((SpriteSourceData *)(arg0 + 0xB0));
    }
}
