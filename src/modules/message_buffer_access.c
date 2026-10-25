/*
 * message_buffer_access — reconstructed translation unit.
 *
 * Availability query and latch reset for the shared message read buffer,
 * original exe:80021FE4..80022014. Both operate on the status byte at primary
 * state +0xCD; the query returns state+0xD8 only while that byte is zero.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "message_buffer_access.h"

s32 *GetAvailableMessageReadBuffer(void) {
    s32 *state;
    u8 busy;
    s32 *buffer;

    state = (s32 *)&D_8006C838;
    busy = *(u8 *)((char *)state + 0xCD);
    buffer = 0;
    if (busy == 0) {
        buffer = (s32 *)((char *)state + 0xD8);
    }
    return buffer;
}

void ClearMessageReadBufferBusy(void) {
    gMessageReadBufferBusy = 0;
}
