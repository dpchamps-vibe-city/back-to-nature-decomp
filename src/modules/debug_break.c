/*
 * debug_break — reconstructed translation unit.
 *
 * Resumable assertion breakpoint used by allocation and range-check loops; no
 * persistent state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "debug_break.h"

/* Returning nonzero keeps the caller's assertion loop active after resume. */
s32 DebugBreakAndReturnTrue(void) {
    M2C_BREAK(1);
    return 1;
}
