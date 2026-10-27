/*
 * Draw the active slot-sprite record selected by the slot texture cache.
 * Packet dimensions/UV/CLUT come from shared work; position and OT entry
 * come from the caller. Contract: ovl11_active_record_sprite_packet.h.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "packet_work.h"
#include "ovl11_active_record_sprite_packet.h"

void Ovl11DrawActiveRecordSpritePacket(void *otEntry, s16 screenX, s16 screenY) {
    SPRT *sprite;
    DR_TPAGE *drawPage;
    u16 pageYBits;
    u16 pageX;

    sprite = (SPRT *)ReservePacketWorkBytes(0x14);
    /* The original reserves twelve bytes even though DR_TPAGE uses eight. */
    drawPage = (DR_TPAGE *)ReservePacketWorkBytes(0xC);
    setSprt(sprite);
    sprite->clut = GetClut(gActiveRecordSpriteDescriptor.clutX, gActiveRecordSpriteDescriptor.clutY);
    setRGB0(sprite, 0x80, 0x80, 0x80);
    setXY0(sprite, screenX, screenY);
    setSemiTrans(sprite, 0);
    setWH(sprite, gActiveRecordSpriteDescriptor.width, gActiveRecordSpriteDescriptor.height);
    setUV0(sprite, (gActiveRecordSpriteDescriptor.textureWordX % 0x40) * 4, gActiveRecordSpriteDescriptor.textureY);
    AddPrim(otEntry, sprite);
    /* Page bits deliberately use the CLUT origin, not textureWordX/Y. */
    pageX = gActiveRecordSpriteDescriptor.clutX;
    pageYBits = (u16)gActiveRecordSpriteDescriptor.clutY & 0xFF00;
    setDrawTPage(drawPage, 1, 1,
                 ((pageYBits & 0x100) >> 4)
                     | ((pageX & 0x3C0) >> 6)
                     | ((pageYBits & 0x200) << 2));
    AddPrim(otEntry, drawPage);
}
