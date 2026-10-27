/*
 * Memory-card command and save-file services for overlay 15.
 *
 * The save menus call these operations one at a time: they share the SDK's
 * outstanding command, a retry counter and the 32-byte filename scratch.
 * Ports 0/1 map to SDK channels 0/0x10. Return values are menu statuses, not
 * unmodified SDK errors; see ovl15_card_service.h for each operation's contract.
 *
 * Retry increments deliberately use the unsigned halfword counter while
 * limits test its signed view. Keep the distinct start/poll paths: the full
 * file operations sample MemCardSync only once per invocation, even on their
 * local retry edges. They are not blocking read/write loops.
 *
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "game_types.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_filename.h"
#include "ovl15_card_service.h"

/* The word-sized port is narrowed before conversion, then reused for the
 * Mcx channel. Menu callers consume the signed-halfword return contract. */
s16 Ovl15ProbeCard(s32 port) {
    long command;
    long result;
    s32 channel;
    s32 status;

    port = (s16) port;
    if ((s16) port == 0) {
        channel = port;
    } else if (port == 1) {
        channel = 0x10;
    } else {
        return 0xC;
    }
    gOvl15CardRetryCount = 0;
    MemCardSync(0, &command, &result);
retry_accept:
    if (MemCardAccept(channel) != 1) {
        return 0xC;
    }
    /* This one-shot Accept/Sync completion scope is matching-sensitive;
     * it does not repeat the wait. */
    do {
        MemCardSync(0, &command, &result);
    } while (0);
    switch (result) {
    case 1:
        if ((s16) gOvl15CardRetryCount >= 8) {
no_card:
            return 4;
        }
        gOvl15CardRetryCount++;
        goto retry_accept;
    case 2:
        if ((s16) gOvl15CardRetryCount >= 8) {
            return 8;
        }
        gOvl15CardRetryCount++;
        goto retry_accept;
    case 4:
        if ((s16) gOvl15CardRetryCount >= 8) {
            gOvl15CardRetryCount = 0;
            port *= 0x10;
retry_unformatted_type:
            status = McxCardType(port);
            if (status != 1) {
                return 0xC;
            }
            McxSync(0, &command, &result);
            switch (result) {
            case 0:
                return 7;
            case 1:
                goto no_card;
            case 2:
                if ((s16) gOvl15CardRetryCount >= 8) {
                    return 6;
                }
                gOvl15CardRetryCount++;
                goto retry_unformatted_type;
            case 3:
                return 5;
            default:
                return 0xC;
            }
        } else {
            gOvl15CardRetryCount++;
            goto retry_accept;
        }
    case 3:
        return 5;
    case 0:
        gOvl15CardRetryCount = 0;
        port *= 0x10;
retry_formatted_type:
        status = McxCardType(port);
        if (status != 1) {
            return 0xC;
        }
        McxSync(0, &command, &result);
        switch (result) {
        case 0:
            return 1;
        case 1:
            goto no_card;
        case 3:
            return 5;
        case 2:
            if ((s16) gOvl15CardRetryCount >= 8) {
                return 0;
            }
            gOvl15CardRetryCount++;
            goto retry_formatted_type;
        default:
            return 0xC;
        }
    default:
        return 0xC;
    }
}

s32 Ovl15PollCardPresence(s16 port) {
    s32 command;
    s32 result;
    s32 syncState;
    s32 channel;
    s32 status;

    channel = 0;
    if (port != 0) {
        channel = 0x10;
        if (port != 1) {
            return 0xC;
        }
    }
    gOvl15CardRetryCount += 1;
    syncState = MemCardSync(1, &command, &result);
    if (syncState == 0) {
        return 3;
    }
    if (syncState == -1) {
        gOvl15CardRetryCount = 0;
        goto request_presence;
    }
    if (command == McFuncExist) {
        status = 5;
        switch (result) {
        case 0:
            return 0;
        case 1:
        case 2:
            break;
        case 3:
            return status;
        default:
            break;
        }
        if ((s16) gOvl15CardRetryCount >= 0x1F) {
            switch (result) {
            case 1:
                return 4;
            case 2:
                return 8;
            }
        }
        MemCardSync(0, &command, &result);
request_presence:
        if (MemCardExist(channel) != 1) {
            return 0xC;
        }
        return 3;
    } else {
        return 3;
    }
}

s32 Ovl15CheckSaveCapacity(s16 port, s16 slot) {
    struct DIRENTRY entries[15];
    long sync[2];
    long matchingFiles;
    long totalFiles;
    long usedBlocks;
    s32 rounded;
    s32 i;
    s32 requiredBlocks;
    s32 channel;
    s32 status;

    requiredBlocks = 4;
    matchingFiles = 0;
    totalFiles = 0;
    if (slot == 3) {
        requiredBlocks = 10;
    }
    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
        } else {
            return status;
        }
    }
    Ovl15BuildCardFilename(Ovl15CardFilename, slot);
    gOvl15CardRetryCount = 0;
    memset(entries, 0, sizeof(entries));
    MemCardSync(0, &sync[0], &sync[1]);
retry_filename:
    sync[1] = MemCardGetDirentry(channel, Ovl15CardFilename, entries, &matchingFiles, 0, 1);
    switch (sync[1]) {
    case 1:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_filename;
        }
        return 4;
    case 2:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_filename;
        }
        return 8;
    case 0:
    default:
        gOvl15CardRetryCount = 0;
        break;
    }
    memset(entries, 0, sizeof(entries));
    MemCardSync(0, &sync[0], &sync[1]);
retry_directory:
    sync[1] = MemCardGetDirentry(channel, "*", entries, &totalFiles, 0, 15);
    switch (sync[1]) {
    case 1:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_directory;
        }
        return 4;
    case 2:
        if ((s16) gOvl15CardRetryCount < 7) {
            u16 nextRetry = gOvl15CardRetryCount + 1;
            gOvl15CardRetryCount = nextRetry;
            goto retry_directory;
        }
        return 8;
    case 0:
    default:
        break;
    }
    usedBlocks = 0;
    for (i = 0; i < 15; i++) {
        long fullBlocks = entries[i].size / 8192;
        if (entries[i].size % 8192) {
            rounded = usedBlocks + 1;
            usedBlocks = rounded + fullBlocks;
        } else {
            usedBlocks += fullBlocks;
        }
    }
    status = 0;
    if (matchingFiles == 0) {
        if (usedBlocks != 15) {
            if ((15 - usedBlocks) < requiredBlocks) {
                return 0xB;
            }
            return 0;
        }
        return 15;
    }
    return status;
}

s32 Ovl15ReadSaveSummary(s16 port, s16 slot, Ovl15SaveSummary *summary) {
    union {
        unsigned long words[32];
        Ovl15SaveSummary summary;
        u8 bytes[0x80];
    } buffer;
    long command;
    long result;
    s32 channel;
    s32 i;
    s32 status;
    u8 checksum;

    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
        } else {
            return status;
        }
    }
    Ovl15BuildCardFilename(Ovl15CardFilename, slot);
    gOvl15CardRetryCount = 0;
    MemCardSync(0, &command, &result);
retry_read:
    MemCardReadFile(channel, Ovl15CardFilename, buffer.words, 0x200, 0x80);
    MemCardSync(0, &command, &result);
    switch (result) {
    case 1:
        if ((s16) gOvl15CardRetryCount >= 7) {
            return 4;
        }
        gOvl15CardRetryCount++;
        goto retry_read;
    case 2:
        if ((s16) gOvl15CardRetryCount >= 7) {
            return 8;
        }
        gOvl15CardRetryCount++;
        goto retry_read;
    case 5:
        if ((s16) gOvl15CardRetryCount < 7) {
            gOvl15CardRetryCount++;
            goto retry_read;
        }
        return 9;
    case 3:
        return 5;
    case 0:
    case 4:
    default:
        /* The caller receives the header even when its checksum fails. */
        *summary = buffer.summary;
        checksum = 0;
        for (i = 0; i < 0x7F; i++) {
            checksum ^= buffer.bytes[i];
        }
        if (checksum != buffer.bytes[0x7F]) {
            return 0xE;
        }
        return 0;
    }
}

s32 Ovl15FormatCard(s16 port) {
    long command;
    long result;
    s32 channel;
    s32 status;

    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
        } else {
            return status;
        }
    }
    gOvl15CardRetryCount = 0;
    MemCardSync(0, &command, &result);
retry_format:
    result = MemCardFormat(channel);
    switch (result) {
    case 1:
        if ((s16) gOvl15CardRetryCount >= 7) {
            return 4;
        }
        gOvl15CardRetryCount++;
        goto retry_format;
    case 2:
        if ((s16) gOvl15CardRetryCount < 7) {
            gOvl15CardRetryCount++;
            goto retry_format;
        }
        return 2;
    case 0:
        return 0;
    default:
        return 2;
    }
}

s32 Ovl15CreateSaveFile(s16 port, s16 slot) {
    s32 command;
    s32 result;
    s32 channel;
    s32 status;

    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
            goto prepare_filename;
        }
        return status;
    }
prepare_filename:
    Ovl15BuildCardFilename(Ovl15CardFilename, (s32) slot);
    gOvl15CardRetryCount = 0;
    MemCardSync(0, &command, &result);
retry_create:
    result = MemCardCreateFile(channel, Ovl15CardFilename, 4);
    switch (result) {
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
        if ((s16) gOvl15CardRetryCount < 7) {
            gOvl15CardRetryCount = (u16) gOvl15CardRetryCount + 1;
            goto retry_create;
        }
        if (result != 1) {
            status = 7;
            if (result != status) {
                return 2;
            }
            status = 0xB;
            return status;
        }
        return 4;
    case 0:
        return 0;
    default:
        status = 2;
        return status;
    }
}

s32 Ovl15WriteSaveFile(s16 port, s16 slot, u32 *data) {
    s32 command;
    s32 result;
    s32 syncState;
    s32 channel;
    s32 status;

    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
            goto prepare_filename;
        }
        return status;
    }
prepare_filename:
    Ovl15BuildCardFilename(Ovl15CardFilename, (s32) slot);
    gOvl15CardRetryCount = 0;
    syncState = MemCardSync(1, &command, &result);
retry_result:
    if (command != McFuncWriteFile) {
        status = 0xC;
        if (syncState != 0) {
            if (syncState > 0) {
                if (syncState != 1) {
                    return status;
                }
                MemCardWriteFile(channel, Ovl15CardFilename, data, 0, 0x8000);
                gOvl15CardRetryCount = 0;
                return 3;
            }
            goto check_idle;
        }
        return status;
    }
    status = 3;
    if (syncState == 0) {
        return status;
    }
    if (syncState > 0) {
        goto check_completion;
    }
    goto check_idle;
check_idle:
    if (syncState == -1) {
        goto start_transfer;
    }
    return 0xC;
check_completion:
    if (syncState != 1) {
        return 0xC;
    }
    goto completed_transfer;
start_transfer:
    MemCardWriteFile(channel, Ovl15CardFilename, data, 0, 0x8000);
    gOvl15CardRetryCount = 0;
    return 3;
completed_transfer:
    switch (result) {
    case 2:
    case 5:
        if ((s16) gOvl15CardRetryCount >= 7) {
            return 2;
        }
        gOvl15CardRetryCount = (u16) gOvl15CardRetryCount + 1;
        goto retry_result;
    case 1:
        if ((s16) gOvl15CardRetryCount < 7) {
            gOvl15CardRetryCount = (u16) gOvl15CardRetryCount + 1;
            goto retry_result;
        }
        return 4;
    case 3:
        return 5;
    case 0:
        return 0;
    default:
        return 0xC;
    }
}

s32 Ovl15ReadSaveFile(s16 port, s16 slot, u32 *data) {
    s32 command;
    s32 result;
    s32 syncState;
    s32 channel;
    s32 status;

    channel = 0;
    if (port != 0) {
        status = 0xC;
        if (port == 1) {
            channel = 0x10;
            goto prepare_filename;
        }
        return status;
    }
prepare_filename:
    Ovl15BuildCardFilename(Ovl15CardFilename, (s32) slot);
    gOvl15CardRetryCount = 0;
    syncState = MemCardSync(1, &command, &result);
retry_result:
    if (command != McFuncReadFile) {
        status = 0xC;
        if (syncState != 0) {
            if (syncState > 0) {
                if (syncState != 1) {
                    return status;
                }
                MemCardReadFile(channel, Ovl15CardFilename, data, 0, 0x8000);
                gOvl15CardRetryCount = 0;
                return 3;
            }
            goto check_idle;
        }
        return status;
    }
    status = 3;
    if (syncState == 0) {
        return status;
    }
    if (syncState > 0) {
        goto check_completion;
    }
    goto check_idle;
check_idle:
    if (syncState == -1) {
        goto start_transfer;
    }
    return 0xC;
check_completion:
    if (syncState != 1) {
        return 0xC;
    }
    goto completed_transfer;
start_transfer:
    MemCardReadFile(channel, Ovl15CardFilename, data, 0, 0x8000);
    gOvl15CardRetryCount = 0;
    return 3;
completed_transfer:
    switch (result) {
    case 1:
        if ((s16) gOvl15CardRetryCount >= 7) {
            return 4;
        }
        gOvl15CardRetryCount = (u16) gOvl15CardRetryCount + 1;
        goto retry_result;
    case 2:
    case 5:
        if ((s16) gOvl15CardRetryCount < 7) {
            gOvl15CardRetryCount = (u16) gOvl15CardRetryCount + 1;
            goto retry_result;
        }
        return 2;
    case 3:
        return 5;
    case 0:
        return 0;
    default:
        return 0xC;
    }
}
