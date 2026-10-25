#ifndef RENDER_PACKET_POOL_H
#define RENDER_PACKET_POOL_H

#include "common.h"

/* Initialize both contexts' 2048-entry secondary OTs and 0x17700-byte
 * packet pools. Requires gGraphicsHeapBase to cover the fixed carved layout;
 * clears each pool and resets its cursor, leaving other context fields alone. */
void InitRenderPacketPools(void);

/* Link the current packet into gActiveRenderContext's secondary OT, advance
 * its cursor by byteCount and return the packet address. Upper OT indices clamp
 * to the last entry; negative indices and capacity are NOT checked. The caller
 * fills the packet (including its length byte) after reserving it. */
s32 LinkAndReservePacket(s32 otIndex, s32 byteCount);

/* Original no-op hook. The hint is ignored; this performs no bounds check. */
void PacketReservationHook(s32 hint);

#endif
