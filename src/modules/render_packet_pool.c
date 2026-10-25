/*
 * Ordering-table packet storage for the two render contexts.
 *
 * Initialization carves fixed pools from gGraphicsHeapBase. Linking uses the
 * active context's secondary OT and advances its packet cursor in bytes;
 * callers remain responsible for capacity and packet contents. Boot owns
 * the context/heap pointers: this unit deliberately accesses them absolutely.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
#include "render_packet_pool.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 LinkAndReservePacket(s32 otIndex, s32 byteCount) {
    struct_8005E3C0 *context;
    s32 *packet;
    s32 *otEntry;
    s32 otCount;

    context = gActiveRenderContext;
    PacketReservationHook(0x80);
    otCount = context->field_110;
    if (otIndex >= otCount) {
        otIndex = otCount - 1;
    }
    packet = (s32 *)context->field_118;
    otEntry = (s32 *)((s32)context->field_120 + (otIndex << 2));
    /* Preserve the packet's length byte, but replace the OT word completely.
     * Unlike addPrim's bitfield store, this deliberately clears the OT's
     * upper byte. Packet contents are filled by the caller after linking. */
    *packet = (*packet & 0xFF000000) | (*otEntry & 0xFFFFFF);
    *otEntry = (s32)packet & 0xFFFFFF;
    context->field_118 = context->field_118 + byteCount;
    return (s32)packet;
}

/* Retained hook, also called by the projected-primitive emitter. The original
 * implementation ignores its hint and performs no capacity check. */
void PacketReservationHook(s32 hint) {
}

void InitRenderPacketPools(void) {
    s32 heapBase;
    s32 contextIndex;
    s32 *contextWords;
    s8 *byte;
    s32 byteIndex;

    heapBase = gGraphicsHeapBase;

    for (contextIndex = 0; contextIndex < 2; contextIndex++) {
        contextWords = (s32 *)((s8 *)&gRenderContexts + contextIndex * 0x134);

        contextWords[0x44] = 0x800;       /* OT entry count */
        contextWords[0x45] = 0xB;
        contextWords[0x48] = heapBase + 0x290 + (contextIndex << 13);
        contextWords[0x49] = heapBase + 0x4290 + (contextIndex * 0x17700);
        contextWords[0x47] = 0x17700;     /* packet-pool capacity, bytes */

        ClearOTagR((unsigned long *)contextWords[0x48], 0x800);

        for (byteIndex = 0; byteIndex <= 0x176FF; byteIndex++) {
            byte = (s8 *)contextWords[0x49] + byteIndex;
            *byte = 0;
        }

        contextWords[0x46] = contextWords[0x49];
    }
}
