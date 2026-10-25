#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

void ovl_11_func_800D92FC(s32 *arg0);
s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
s32 ovl_11_func_800D95EC(s32 arg0, s32 arg1, s32 *, s32 *, s32 *);
s32 ovl_11_func_800D981C(s32 arg0, s32 arg1, s32 *, s32 *, s32 *);
s32 ovl_11_func_800D9B14(s32 arg0, s32 arg1, s32 *, s32 *, s32 *);
s32 ovl_11_func_800D9E10(s32 arg0, u8, s32 *, s32 *, s32 *);

void ovl_11_func_800D93C8(void) {
    s32 bounds[4];
    s16 pos[4];
    s32 projected[4];
    s32 sprite;
    s32 texture;
    s32 yOffset;
    s32 y;
    s32 x;

    if (!(((Ovl11D93C8StateView *)D_8006C838)->flags & 0x200)) {
        ovl_11_func_800D92FC(bounds);
        if (((Ovl11D93C8StateView *)D_8006C838)->mode == 0) {
            D_80128DA8 = ovl_11_func_800D95EC;
        } else if (((Ovl11D93C8StateView *)D_8006C838)->mode == 1) {
            D_80128DA8 = ovl_11_func_800D981C;
        } else if (((Ovl11D93C8StateView *)D_8006C838)->mode == 2) {
            D_80128DA8 = ovl_11_func_800D9B14;
        } else {
            D_80128DA8 = ovl_11_func_800D9E10;
        }
        for (y = bounds[1]; y < bounds[3]; y++) {
            for (x = bounds[0]; x < bounds[2]; x++) {
                pos[0] = x * 400 - 4050;
                pos[1] = -100;
                pos[2] = 1250 - y * 400;
                if (ovl_11_func_800F5888((u16 *)pos, projected) != 0 &&
                    D_80128DA8(((Ovl11D93C8MapView *)D_8006C838)->cells[y][x].kind,
                        ((Ovl11D93C8MapView *)D_8006C838)->cells[y][x].value,
                        &sprite, &texture, &yOffset) != 0) {
                    DrawSpriteFrameRects(gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4),
                        (s32)((u8 *)D_8006C838 + 0x3BC8 + texture * 0x30),
                        (u8)sprite, 0, (s16)projected[0], (s16)(projected[1] + yOffset));
                }
            }
        }
    }
}
