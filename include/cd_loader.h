#ifndef CD_LOADER_H
#define CD_LOADER_H

#include "common.h"

/*
 * Single-drive archive loader. Initialize the 33-entry byte-offset index from
 * A_FILE.HDT and locate A_FILE.BIN before reading entries. Entry indices and
 * byte ranges are unchecked. All operations share the drive and request state;
 * do not interleave requests from different APIs. Buffers must accommodate the
 * whole sector transfer, not just the requested payload. Addresses retain the
 * original s32 ABI where used below.
 */

/* exe:0x800147BC. Uses D_8006C838 as temporary index storage; retries forever
 * until both files can be found. Returns zero. */
s32 CdInitArchiveIndex(void);

/* exe:0x800145F0. Load [firstEntry, endEntry) synchronously, retrying errors.
 * Read whole sectors into sectorBuffer, then compact the exact byte range to
 * destination if necessary. Both addresses may be equal. Returns zero. */
s32 CdLoadArchiveRangeBlocking(s32 firstEntry, s32 endEntry,
                               s32 destination, s32 sectorBuffer);

/* exe:0x80014854 / 0x80014B44. Separate asynchronous range-read channel:
 * Start returns 0 when accepted, 1 if already busy. It reads to destination
 * minus the first entry's sector offset. Poll returns -1 if idle, 0 while
 * pending (including an error/retry), 1 on completion. The completion query
 * (exe:0x80014B38) reads the retained completion flag without polling. */
s32 CdStartArchiveRangeRead(s32 firstEntry, s32 endEntry, s32 destination);
s32 CdPollArchiveRangeRead(void);
s32 CdGetRangeReadComplete(void);

/* exe:0x80014988. Start/poll a sector-oriented archive read. restart == 1
 * replaces the current request. Returns 0 while pending, 1 on completion,
 * -1 for the idle fallback. Entry is truncated to s16; entry offset and
 * byteOffset are separately rounded down to sectors. Reads ceil(byteCount /
 * 2048) + 1 sectors, so the destination needs that extra sector of capacity. */
s32 CdReadArchiveSectors(s32 entry, s32 byteOffset, s32 byteCount,
                         s32 destination, s32 restart);

/* exe:0x80014BCC. Read floor(byteCount / 2048) sectors synchronously at
 * archiveBase + floor(byteOffset / 2048), retrying errors. unused is ignored;
 * mode == -1 only sets the drive location and performs no read. */
void CdReadDiscSectorsBlocking(s32 unused, s32 byteOffset, s32 byteCount,
                              s32 mode, s32 destination);

/* exe:0x80014CBC. Start/poll a byte-exact archive subrange. restart == 1
 * starts a new transfer; otherwise call repeatedly with the same arguments.
 * NULL means pending/reissued. On completion compact == 1 moves the bytes to
 * buffer and returns buffer; otherwise returns buffer + leading sector bytes.
 * Capacity must include those leading bytes and the rounded-up final sector.
 * exe:0x80014CB0 queries the retained byte-read busy flag without polling. */
u8 *CdReadArchiveBytes(s32 entry, s32 byteOffset, s32 byteCount,
                       u8 *buffer, s32 compact, s32 restart);
s32 CdIsByteRangeReadIdle(void);

#endif
