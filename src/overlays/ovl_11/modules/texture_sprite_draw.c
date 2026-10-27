/* Draw existing texture-cache records without loading or reclaiming them.
 * The sprite path commits SPRT/DR_MODE packets to packet_work; the quad path
 * advances the active context's POLY_FT4 cursor. Ownership stays external.
 * Membership/boundary evidence: notes/file-groupings.md. */
#include "texture_sprite_draw.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "packet_work.h"
#include "ovl11_slot_decorations.h"

void Ovl11DrawTextureSpriteInCell(s32 *ot, s16 textureIndex, s16 x, s16 y) {
    SPRT *packet;
    s32 textureX;
    s32 textureY;

    Ovl11DrawEmptySlot((s32) (ot + 1), x, y);
    if (gOvl11TextureCache[textureIndex].width < 32) {
        x += (32 - gOvl11TextureCache[textureIndex].width) / 2;
    }
    if (gOvl11TextureCache[textureIndex].height < 32) {
        y += (32 - gOvl11TextureCache[textureIndex].height) / 2;
    }
    packet = (SPRT *) ReservePacketWorkBytes(0);
    setSprt(packet);
    /* Keep signed CLUT coordinate loads in this SDK-function path. The
     * sibling quad uses the SDK macro and the record's unsigned fields. */
    packet->clut = GetClut(*(s16 *) &gOvl11TextureCache[textureIndex].clutX,
                          *(s16 *) &gOvl11TextureCache[textureIndex].clutY);
    setRGB0(packet, 128, 128, 128);
    setXY0(packet, x, y);
    setSemiTrans(packet, 0);
    setWH(packet, gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].height);
    setUV0(packet, (gOvl11TextureCache[textureIndex].x - 320) * 4, gOvl11TextureCache[textureIndex].y);
    addPrim(ot, packet);
    packet++;
    setDrawTPage((DR_MODE *) packet, 1, 1,
        (textureY = gOvl11TextureCache[textureIndex].y,
         textureX = gOvl11TextureCache[textureIndex].x,
         getTPage(0, 0, textureX & 0x3C0, textureY)));
    addPrim(ot, packet);
    SetPacketWorkCursor((s32) ((DR_MODE *) packet + 1));
}

void Ovl11DrawScaledTextureQuad(s32 *ot, s16 textureIndex, s16 centreX, s16 centreY, s32 scale4096) {
    POLY_FT4 *packet;
    s16 width;
    s16 height;
    s16 left;
    s16 top;
    s32 textureX;
    s32 textureY;
    s32 page;

    width = gOvl11TextureCache[textureIndex].width;
    height = gOvl11TextureCache[textureIndex].height;
    /* Division happens BEFORE multiplication: only integer scale multiples
     * survive, unlike a usual Q12 multiply-then-shift. */
    width *= scale4096 / 4096;
    height *= scale4096 / 4096;
    packet = (POLY_FT4 *) gActiveRenderContext->field_118;
    setPolyFT4(packet);
    packet->clut = getClut(gOvl11TextureCache[textureIndex].clutX, gOvl11TextureCache[textureIndex].clutY);
    setRGB0(packet, 128, 128, 128);
    setSemiTrans(packet, 0);
    textureX = gOvl11TextureCache[textureIndex].x;
    textureY = gOvl11TextureCache[textureIndex].y;
    page = getTPage(0, 0, textureX & 0x3C0, textureY & 0x100);
    packet->tpage = page | getTPage(0, 0, 0, (u16) gOvl11TextureCache[textureIndex].y & 0x200);
    setUV4(packet,
        gOvl11TextureCache[textureIndex].x, gOvl11TextureCache[textureIndex].y,
        gOvl11TextureCache[textureIndex].x + gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].y,
        gOvl11TextureCache[textureIndex].x, gOvl11TextureCache[textureIndex].y + gOvl11TextureCache[textureIndex].height,
        gOvl11TextureCache[textureIndex].x + gOvl11TextureCache[textureIndex].width, gOvl11TextureCache[textureIndex].y + gOvl11TextureCache[textureIndex].height);
    left = centreX - width / 2;
    top = centreY - height / 2;
    setXYWH(packet, left, top, width, height);
    addPrim(ot, packet);
    gActiveRenderContext->field_118 = (s32) (packet + 1);
}
