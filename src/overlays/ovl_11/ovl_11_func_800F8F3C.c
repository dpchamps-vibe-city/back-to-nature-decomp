#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void ovl_11_func_800F8F3C(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4) {
    POLY_FT4 *p;
    s16 width;
    s16 height;
    s16 left;
    s16 top;
    s32 textureX;
    s32 textureY;
    s32 page;

    width = D_80129648_textures[arg1].width;
    height = D_80129648_textures[arg1].height;
    width *= arg4 / 4096;
    height *= arg4 / 4096;
    p = (POLY_FT4 *) gActiveRenderContext->field_118;
    setPolyFT4(p);
    p->clut = getClut(D_80129648_textures[arg1].clutX, D_80129648_textures[arg1].clutY);
    setRGB0(p, 128, 128, 128);
    setSemiTrans(p, 0);
    textureX = D_80129648_textures[arg1].x;
    textureY = D_80129648_textures[arg1].y;
    page = getTPage(0, 0, textureX & 0x3C0, textureY & 0x100);
    p->tpage = page | getTPage(0, 0, 0, (u16) D_80129648_textures[arg1].y & 0x200);
    setUV4(p,
        D_80129648_textures[arg1].x, D_80129648_textures[arg1].y,
        D_80129648_textures[arg1].x + D_80129648_textures[arg1].width, D_80129648_textures[arg1].y,
        D_80129648_textures[arg1].x, D_80129648_textures[arg1].y + D_80129648_textures[arg1].height,
        D_80129648_textures[arg1].x + D_80129648_textures[arg1].width, D_80129648_textures[arg1].y + D_80129648_textures[arg1].height);
    left = arg2 - width / 2;
    top = arg3 - height / 2;
    setXYWH(p, left, top, width, height);
    addPrim(arg0, p);
    gActiveRenderContext->field_118 = (s32) (p + 1);
}
