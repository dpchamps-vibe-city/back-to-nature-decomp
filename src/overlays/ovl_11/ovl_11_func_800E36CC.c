#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

s32 ovl_11_func_800E39B8(s32 *arg0, u32 arg1);

u8 *ovl_11_func_800E36CC(u16 *arg0, s16 arg1, s16 arg2, u32 arg3) {
    Ovl11Rect801248B4 rect;
    Ovl11Rect801248B4 bounds;
    s32 row;
    s32 col;
    s32 start;
    s32 end;
    u32 size;
    s32 height;
    u8 *data;
    u8 *dest;

    if (arg3 >= 0x32U) {
        return NULL;
    }
    start = _D_801247E8[arg3];
    size = _D_801247E8[arg3 + 1] - start;
    rect = D_801248B4;
    rect.x = arg1 + rect.x;
    rect.y = arg2 + rect.y;
    bounds = rect;
    col = start / 2 % rect.w;
    row = start / 2 / rect.w;
    end = (s32)(start + size) / 2;
    height = end / rect.w - row;
    if (end % rect.w != 0) {
        height++;
    }
    rect.h = height;
    rect.y += row;
    if (rect.y + rect.h > bounds.y + bounds.h) {
        if (rect.y < bounds.y + bounds.h) {
            rect.h -= rect.y + rect.h - (bounds.y + bounds.h);
            StoreImage((RECT *)&rect, (u_long *)arg0);
            DrawSync(0);
            data = (u8 *)arg0 + col * 2;
            dest = (u8 *)arg0 + rect.w * rect.h * 2;
            memcpy(dest,
                   (void *)D_801294E8,
                   _D_801247E8[arg3 + 1] - D_801248B4.w * D_801248B4.h * 2);
        } else {
            start -= D_801248B4.w * D_801248B4.h * 2;
            data = (u8 *)D_801294E8 + start;
        }
    } else {
        StoreImage((RECT *)&rect, (u_long *)arg0);
        DrawSync(0);
        data = (u8 *)arg0 + col * 2;
    }
    if (D_80129420[arg3] != ovl_11_func_800E39B8((s32 *)data, size)) {
        return NULL;
    }
    return data;
}
