#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Build four UV corners and their texture-page/CLUT words. */
void func_8001DBF0(TextureRegion *region, s32 x, s32 y, s32 width,
                   s32 height, s32 clutX, s32 clutY) {
    setUVWH(region, (x % 64) * 4, y % 256, width - 1, height - 1);
    region->tpage = GetTPage(0, 0, x, y);
    region->clut = GetClut(clutX, clutY);
}
