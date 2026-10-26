/*
 * Forward element-copy helpers for the overlay's resource installers.
 * Count units differ by entry point; neither function implements memmove.
 * Contract: ovl11_block_copy.h. Original ovl_11:800DD21C-800DD274.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_block_copy.h"

void Ovl11CopyWordBlock(s32 *destination, s32 *source, u32 count) {
    s32 *writeCursor;
    s32 *readCursor;
    s32 value;
    u32 copied;

    writeCursor = destination;
    readCursor = source;
    copied = 0;
    if (count != 0) {
        do {
            value = *readCursor;
            readCursor++;
            copied += 1;
            *writeCursor = value;
            writeCursor++;
        } while (copied < count);
    }
}

void Ovl11CopyHalfwordBlock(u16 *destination, u16 *source, u32 count) {
    u16 *writeCursor;
    u16 *readCursor;
    u16 value;
    u32 copied;

    writeCursor = destination;
    readCursor = source;
    copied = 0;
    if (count != 0) {
        do {
            value = *readCursor;
            readCursor += 1;
            copied += 1;
            *writeCursor = value;
            writeCursor += 1;
        } while (copied < count);
    }
}
