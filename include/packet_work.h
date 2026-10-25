#ifndef PACKET_WORK_H
#define PACKET_WORK_H

#include "common.h"

/*
 * Packet-work cursor for the active, double-buffered render context.
 * Boot/heap setup and the frame reset must initialize the context and libgs
 * work base before use. Switching contexts/resetting a frame invalidates the
 * caller's outstanding cursor; these functions do not select a context.
 *
 * Addresses use the existing s32 ABI; counts and cursor arithmetic are bytes,
 * not 32-bit GPU words. No alignment, lower-bound or arithmetic-wrap check is
 * performed. Callers must supply valid, non-wrapping addresses/counts, ensure
 * sufficient room before writing, and arrange any packet alignment themselves.
 * The exclusive buffer end is allowed as a cursor but cannot be written to.
 * Exceeding it enters the debug assertion loop, not a recoverable error return.
 */

/* Return the old cursor and advance it by byteCount (normally nonnegative).
 * A zero count queries the cursor without reserving space. A common caller
 * builds a variable-size packet there, then commits its end with the setter.
 * Original identity: exe:0x80011F5C (formerly func_80011F5C). */
s32 ReservePacketWorkBytes(s32 byteCount);

/* Install an absolute byte address as the libgs cursor, checking only the
 * upper bound. Does not validate/write packets or prevent backward movement.
 * Original identity: exe:0x80011FD8 (formerly func_80011FD8). */
void SetPacketWorkCursor(s32 cursor);

#endif
