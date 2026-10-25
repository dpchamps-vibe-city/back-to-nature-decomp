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
    if (gOvl11TextureCache[arg1].width < 32) {
        arg2 += (32 - gOvl11TextureCache[arg1].width) / 2;
    }
    if (gOvl11TextureCache[arg1].height < 32) {
        arg3 += (32 - gOvl11TextureCache[arg1].height) / 2;
    }
    p = (SPRT *) ReservePacketWorkBytes(0);
    setSprt(p);
    p->clut = GetClut(*(s16 *) &gOvl11TextureCache[arg1].clutX,
                     *(s16 *) &gOvl11TextureCache[arg1].clutY);
    setRGB0(p, 128, 128, 128);
    setXY0(p, arg2, arg3);
    setSemiTrans(p, 0);
    setWH(p, gOvl11TextureCache[arg1].width, gOvl11TextureCache[arg1].height);
    setUV0(p, (gOvl11TextureCache[arg1].x - 320) * 4, gOvl11TextureCache[arg1].y);
    addPrim(arg0, p);
    p++;
    setDrawTPage((DR_MODE *) p, 1, 1,
        (textureY = gOvl11TextureCache[arg1].y,
         textureX = gOvl11TextureCache[arg1].x,
         getTPage(0, 0, textureX & 0x3C0, textureY)));
    addPrim(arg0, p);
    SetPacketWorkCursor((s32) ((DR_MODE *) p + 1));
}
