/*
 * Upper-bound checks for the active render context's libgs packet cursor.
 * Reconstructed slice of the boot/render family, not an original TU boundary;
 * membership evidence is recorded in notes/file-groupings.md.
 */

#include "common.h"
#include "packet_work.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"

/* Shared with boot/render code, not private state. The tentative definition
 * preserves the original GP-relative access to the active context pointer. */
struct_8005E3C0 *D_8005E3C0;

s32 func_800129E8(void);

s32 ReservePacketWorkBytes(s32 byteCount) {
    s32 bufferEnd;
    s32 cursor;

    bufferEnd = D_8005E3C0->packetWorkBase + D_8005E3C0->packetWorkCapacityBytes;
    cursor = (s32)GsGetWorkBase();
    if ((u32)bufferEnd < (u32)(cursor + byteCount)) {
        /* The assertion helper breaks and returns nonzero: overflow normally
         * stays here. Preserve the error call if execution ever continues. */
        do {
        } while (func_800129E8() != 0);
        SystemError(0x50, 0);
    }
    GsSetWorkBase((PACKET *)(cursor + byteCount));
    return cursor;
}

void SetPacketWorkCursor(s32 cursor) {
    if ((u32)(D_8005E3C0->packetWorkBase + D_8005E3C0->packetWorkCapacityBytes) < (u32)cursor) {
        do {
        } while (func_800129E8() != 0);
    }
    GsSetWorkBase((PACKET *)cursor);
}
