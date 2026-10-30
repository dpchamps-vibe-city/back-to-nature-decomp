/*
 * CD archive loading: initialize byte offsets from A_FILE.HDT, locate
 * A_FILE.BIN, and provide blocking and start/poll reads over its contents.
 * Sector units are 2048 bytes. See cd_loader.h for buffer and retry contracts.
 *
 * Original order: exe:0x80014554-0x80014E90. The shared GP-relative drive
 * location, base sector, and read-state cluster establish this unit; the
 * filename loader is used only by its index initializer. The intervening dead
 * 0x80014748 variant is retained. Membership evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "cd_loader.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "cd_audio.h"

/* Tentative definitions preserve this unit's original GP-relative ownership.
 * Location is four bytes of CdlLOC storage, retaining its existing word type. */
s32 gCdReadLocation;
s32 gCdRangeReadBusy;
s32 gCdRangeFirstEntry;
s32 gCdRangeEndEntry;
s32 gCdRangeDestination;
s32 gCdRangeLeadingBytes;
s32 gCdRangeEntry;
s32 gCdRangeReadComplete;
volatile s32 gCdArchiveReadBusy;
s16 gCdReadEntry;
s32 gCdReadByteOffset;
s32 gCdReadDestination;
s32 gCdReadByteCount;
s32 gCdReadSector;
s32 gCdArchiveBaseSector;
s32 gCdByteRangeReadBusy;

/* exe:0x80014554. Internal filename-based bootstrap; never narrows linkage. */
s32 CdLoadFileBlocking(s32 filename, s32 destination) {
    CdlFILE file;
    s32 sectors;
    s32 result;

    ResetCallback();
    DrawSync(0);
    do {
        result = CdSearchFile(&file, (char *)filename);
    } while (result == 0);
    sectors = (file.size + 0x7FF) >> 11;
    CdControl(CdlSetloc, (u_char *)&file, 0);
    CdRead(sectors, (u_long *)destination, CdlModeSpeed);
    while (CdReadSync(1, 0) > 0) {
        VSync(0);
    }
    return 0;
}

s32 CdLoadArchiveRangeBlocking(s32 firstEntry, s32 endEntry,
                               s32 destination, s32 sectorBuffer) {
    u32 startPosition, endPosition;
    s32 byteCount, sectors;
    u_char mode;
    s32 *startOffset, *endOffset;

    ResetCallback();
    DrawSync(0);
    /* This original interior-symbol view selects the last word of each
     * 0x28-byte entry. Keep its signed loads and address formation. */
    endOffset = (s32 *)((char *)&gCdArchiveFirstByteOffset + endEntry * 0x28);
    startOffset = (s32 *)((char *)&gCdArchiveFirstByteOffset + firstEntry * 0x28);
    do {
        startPosition = *startOffset;
        endPosition = *endOffset;
        gCdRangeLeadingBytes = startPosition & 0x7FF;
        gCdReadSector = (startPosition >> 11) + gCdArchiveBaseSector;
        sectors = (endPosition >> 11) - (startPosition >> 11);
        sectors += (endPosition & 0x7FF) != 0;
        byteCount = endPosition - startPosition;
        CdSync(0, 0);
        CdControl(CdlSetloc, CdIntToPos(gCdReadSector, (CdlLOC *)&gCdReadLocation), 0);
        mode = CdlModeSpeed;
        do {
            if (CdControl(CdlSetmode, &mode, 0) == 0) continue;
            break;
        } while (1);
        VSync(3);
        CdRead(sectors, (u_long *)sectorBuffer, CdlModeSpeed);
    } while (CdReadSync(0, 0) != 0);
    if (destination != sectorBuffer + gCdRangeLeadingBytes) {
        memmove(destination, sectorBuffer + gCdRangeLeadingBytes, byteCount);
    }
    return 0;
}

/* Return an archive entry's byte length rounded up to a four-byte boundary. */
s32 func_80014748(s32 entry) {
    u32 startByte;
    u32 endByte;
    u32 currentByteOffset;
    u32 nextByteOffset;
    u32 *offsets;

    ResetCallback();
    DrawSync(0);
    offsets = (u32 *)&gCdArchiveEntries[0];
    /* Each index record occupies 40 bytes; its byte offset is at +0x24. */
    currentByteOffset = entry << 2;
    currentByteOffset += entry;
    currentByteOffset <<= 3;
    entry++;
    nextByteOffset = entry << 2;
    nextByteOffset += entry;
    nextByteOffset <<= 3;
    offsets += 9;
    endByte = *(u32 *)((u8 *)offsets + nextByteOffset);
    startByte = *(u32 *)((u8 *)offsets + currentByteOffset);
    return (endByte - startByte + 3) / 4 * 4;
}

s32 CdInitArchiveIndex(void) {
    CdlFILE archive;
    s32 *source;
    s32 *destination;
    s32 i;
    s32 baseSector;
    char *filename;

    filename = gCdArchiveDataPath;
    CdLoadFileBlocking((s32)&gCdArchiveIndexPath, (s32)&D_8006C838);

    destination = (s32 *)&gCdArchiveEntries;
    destination = (s32 *)((unsigned char *)destination + 0x24);
    source = (s32 *)&D_8006C838;
    for (i = 0; i < 0x21; i++) {
        *destination = *source;
        source++;
        destination += 0x28 / 4;
    }

    do {
        source = (s32 *)CdSearchFile(&archive, filename);
    } while (source == 0);

    baseSector = CdPosToInt(&archive);
    gCdReadSector = baseSector;
    gCdArchiveBaseSector = baseSector;

    return 0;
}

s32 CdStartArchiveRangeRead(s32 firstEntry, s32 endEntry, s32 destination) {
    u32 startPosition;
    u32 endPosition;
    s32 sectors;
    s32 sectorDestination;

    startPosition = gCdArchiveEntries[firstEntry].loc;
    endPosition = gCdArchiveEntries[endEntry].loc;
    gCdRangeEntry = firstEntry;
    gCdRangeLeadingBytes = startPosition & 0x7FF;
    gCdReadSector = (startPosition >> 11) + gCdArchiveBaseSector;
    sectors = (endPosition >> 11) - (startPosition >> 11);
    sectors += (endPosition & 0x7FF) != 0;

    if (gCdRangeReadBusy != 0) {
        goto busy;
    }

    for (;;) {
        CdSync(0, 0);
        gCdRangeFirstEntry = firstEntry;
        gCdRangeEndEntry = endEntry;
        gCdRangeDestination = destination;
        sectorDestination = destination - gCdRangeLeadingBytes;
        if (CdControl(CdlSetloc, CdIntToPos(gCdReadSector, (CdlLOC *)&gCdReadLocation), 0) != 0 &&
            CdRead(sectors, (u_long *)sectorDestination, CdlModeSpeed) != 0) {
            gCdRangeReadBusy = 1;
            gCdRangeReadComplete = 0;
            break;
        }
    }

    return 0;

busy:
    return 1;
}

s32 CdReadArchiveSectors(s32 entry, s32 byteOffset, s32 byteCount,
                         s32 destination, s32 restart) {
    s16 index;
    s32 sectorCount;
    s32 sectors;
    s32 one;
    u_char mode;

    index = (s16)entry;
    one = 1;
    sectorCount = ((u32)byteCount + 0x7FF) >> 11;
    sectors = sectorCount + 1;
    gCdReadSector = (gCdArchiveEntries[index].loc >> 11) + ((u32)byteOffset >> 11) + gCdArchiveBaseSector;

    if (gCdArchiveReadBusy == 0 || restart == one) {
        for (;;) {
            CdReadBreak();
            CdFlush();
            CdSync(0, 0);
            gCdReadEntry = index;
            gCdReadByteOffset = byteOffset;
            gCdReadDestination = destination;
            gCdReadByteCount = byteCount;
            if (CdControl(CdlSetloc, CdIntToPos(gCdReadSector, (CdlLOC *)&gCdReadLocation), 0) != 0) {
                mode = CdlModeSpeed;
                do {
                    if (CdControl(CdlSetmode, &mode, 0) == 0) continue;
                    break;
                } while (1);
                VSync(3);
                if (CdRead(sectors, (u_long *)destination, CdlModeSpeed) != 0) break;
            }
        }
        gCdArchiveReadBusy = 1;
        return 0;
    }

    /* Retain the second volatile observation and its idle fallback. */
    if (gCdArchiveReadBusy == 0) goto fail;

    {
        s32 result = CdReadSync(1, 0);
        if (result != -1) {
            if (result != 0) return 0;
            gCdArchiveReadBusy = 0;
            return 1;
        }
        gCdArchiveReadBusy = 0;
        return CdReadArchiveSectors(index, byteOffset, byteCount, destination, restart);
    }

fail:
    return -1;
}

s32 CdGetRangeReadComplete(void) {
    return gCdRangeReadComplete;
}

s32 CdPollArchiveRangeRead(void) {
    s32 result;

    if (gCdRangeReadBusy != 0) {
        result = CdReadSync(1, 0);
        if (result == -1) goto retry;
        if (result != 0) goto pending;
        gCdRangeReadBusy = 0;
        gCdRangeReadComplete = 1;
        return 1;
retry:
        gCdRangeReadBusy = 0;
        CdStartArchiveRangeRead(gCdRangeFirstEntry, gCdRangeEndEntry, gCdRangeDestination);
        gCdRangeReadComplete = 0;
        return 0;
pending:
        gCdRangeReadComplete = 0;
        return 0;
    }
    return -1;
}

void CdReadDiscSectorsBlocking(s32 unused, s32 byteOffset, s32 byteCount,
                              s32 mode, s32 destination) {
    u_char readMode;
    s32 firstSector;
    s32 sectors;

    firstSector = (u32)byteOffset >> 11;
    sectors = (u32)byteCount >> 11;
    for (;;) {
        gCdReadSector = firstSector + gCdArchiveBaseSector;
        CdReadBreak();
        CdFlush();
        CdSync(0, 0);
        CdControl(CdlSetloc, CdIntToPos(gCdReadSector, (CdlLOC *)&gCdReadLocation), 0);
        if (mode != -1) {
            readMode = CdlModeSpeed;
            do {
            } while (CdControl(CdlSetmode, &readMode, 0) == 0);
            VSync(3);
            CdRead(sectors, (u_long *)destination, CdlModeSpeed);
            if (CdReadSync(0, 0) != 0) {
                continue;
            }
        }
        break;
    }
}

s32 CdIsByteRangeReadIdle(void) {
    return gCdByteRangeReadBusy == 0;
}

u_char *CdReadArchiveBytes(s32 entry, s32 byteOffset, s32 byteCount,
                           u_char *buffer, s32 compact, s32 restart) {
    u32 position;
    u32 endPosition;
    u_char *result;
    u_char *source;
    s32 firstSector;
    s32 leadingBytes;
    s32 endSector;
    s32 sectorCount;
    s32 syncResult;
    s32 one;
    s32 startRequest;
    s32 length;

    position = gCdArchiveEntries[entry].loc + byteOffset;
    length = byteCount;
    result = NULL;
    startRequest = restart;
    endPosition = position + length;
    endSector = endPosition >> 11;
    endSector += (endPosition & 0x7FF) != 0;
    firstSector = position >> 11;
    leadingBytes = position & 0x7FF;
    sectorCount = endSector - firstSector;

    if (gCdArchiveReadBusy == 0 || (one = 1, startRequest == one)) {
        StopCdAudio();
        gCdReadSector = firstSector + gCdArchiveBaseSector;
        CdReadBreak();
        CdFlush();
        CdSync(0, 0);
        CdControl(CdlSetloc, (u_char *)CdIntToPos(gCdReadSector, (CdlLOC *)&gCdReadLocation), 0);
        CdRead(sectorCount, (u_long *)buffer, CdlModeSpeed);
        gCdArchiveReadBusy = 1;
        if (startRequest == 1) {
            gCdByteRangeReadBusy = 1;
        }
        goto done;
    }

    syncResult = CdReadSync(1, 0);
    if (syncResult == -1) {
        goto retry;
    }
    if (syncResult != 0) {
        goto pending;
    }

    if (compact == one) {
        source = buffer + leadingBytes;
        if (buffer != source) {
            memmove(buffer, source, length);
        }
        result = buffer;
    } else {
        result = buffer + leadingBytes;
    }
    gCdArchiveReadBusy = 0;
    gCdByteRangeReadBusy = 0;
    goto done;

retry:
    gCdArchiveReadBusy = 0;
    /* Reissue on failure, but deliberately ignore the recursive result:
     * completion is observed only by a subsequent external poll. */
    CdReadArchiveBytes(entry, byteOffset, length, buffer, compact, startRequest);
    goto pending;

pending:
    result = NULL;
    gCdByteRangeReadBusy = 1;

done:
    return result;
}
