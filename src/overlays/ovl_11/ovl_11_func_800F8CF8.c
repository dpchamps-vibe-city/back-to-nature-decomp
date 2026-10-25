#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "packet_work.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);

void ovl_11_func_800F8CF8(s32 *arg0, s16 arg1, s16 arg2, s16 arg3) {
    SPRT *p;
    s32 textureX;
    s32 textureY;

    ovl_11_func_800F7010((s32) (arg0 + 1), arg2, arg3);
    if (D_80129648_textures[arg1].width < 32) {
        arg2 += (32 - D_80129648_textures[arg1].width) / 2;
    }
    if (D_80129648_textures[arg1].height < 32) {
        arg3 += (32 - D_80129648_textures[arg1].height) / 2;
    }
    p = (SPRT *) ReservePacketWorkBytes(0);
    setSprt(p);
    p->clut = GetClut(*(s16 *) &D_80129648_textures[arg1].clutX,
                     *(s16 *) &D_80129648_textures[arg1].clutY);
    setRGB0(p, 128, 128, 128);
    setXY0(p, arg2, arg3);
    setSemiTrans(p, 0);
    setWH(p, D_80129648_textures[arg1].width, D_80129648_textures[arg1].height);
    setUV0(p, (D_80129648_textures[arg1].x - 320) * 4, D_80129648_textures[arg1].y);
    addPrim(arg0, p);
    p++;
    setDrawTPage((DR_MODE *) p, 1, 1,
        (textureY = D_80129648_textures[arg1].y,
         textureX = D_80129648_textures[arg1].x,
         getTPage(0, 0, textureX & 0x3C0, textureY)));
    addPrim(arg0, p);
    SetPacketWorkCursor((s32) ((DR_MODE *) p + 1));
}
