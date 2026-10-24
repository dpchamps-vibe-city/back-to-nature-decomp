#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"

/* GP-relative CD loading state shared with the other CD helpers. */
s32 D_8005E3F0;
s32 D_8005E410;
s32 D_8005E428;
s32 D_8005E430;
s32 D_8005E2B4;

void func_80021B20(void);

/* Start or poll a byte-range read. A completed read either returns a pointer
 * past the leading sector bytes, or moves the requested bytes to buffer when
 * compact == 1. restart == 1 forces a new request. A failed read is reissued;
 * its recursive result is deliberately ignored until the caller polls again. */
u_char *func_80014CBC(s32 file_index, s32 file_offset, s32 byte_count,
                     u_char *buffer, s32 compact, s32 restart) {
    u32 position;
    u32 end_position;
    u_char *result;
    u_char *src;
    s32 first_sector;
    s32 sector_offset;
    s32 end_sector;
    s32 sector_count;
    s32 sync_result;
    s32 one;
    s32 start_request;
    s32 len;

    position = D_80048B1C[file_index].loc + file_offset;
    len = byte_count;
    result = NULL;
    start_request = restart;
    end_position = position + len;
    /* CD data sectors contain 2048 bytes; round the end sector upward. */
    end_sector = end_position >> 11;
    end_sector += (end_position & 0x7FF) != 0;
    first_sector = position >> 11;
    sector_offset = position & 0x7FF;
    sector_count = end_sector - first_sector;

    if (D_8005E410 == 0 || (one = 1, start_request == one)) {
        func_80021B20();
        D_8005E428 = first_sector + D_8005E430;
        CdReadBreak();
        CdFlush();
        CdSync(0, 0);
        CdControl(CdlSetloc, (u_char *)CdIntToPos(D_8005E428, (CdlLOC *)&D_8005E3F0), 0);
        CdRead(sector_count, (u_long *)buffer, CdlModeSpeed);
        D_8005E410 = 1;
        if (start_request == 1) {
            D_8005E2B4 = 1;
        }
        goto done;
    }

    sync_result = CdReadSync(1, 0);
    if (sync_result == -1) {
        goto retry;
    }
    if (sync_result != 0) {
        goto zero_tail;
    }

    if (compact == one) {
        src = buffer + sector_offset;
        if (buffer != src) {
            memmove(buffer, src, len);
        }
        result = buffer;
    } else {
        result = buffer + sector_offset;
    }
    D_8005E410 = 0;
    D_8005E2B4 = 0;
    goto done;

retry:
    D_8005E410 = 0;
    func_80014CBC(file_index, file_offset, len, buffer, compact, start_request);
    goto zero_tail;

zero_tail:
    result = NULL;
    D_8005E2B4 = 1;

done:
    return result;
}
