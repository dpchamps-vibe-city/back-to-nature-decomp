#ifndef VBLANK_TIMER_H
#define VBLANK_TIMER_H

#include "common.h"

/* Nonblocking interval test in the SDK VSync(-1) counter's units, not game
 * updates or milliseconds. *stamp is caller-owned writable state. Zero means
 * uninitialized: this call replaces it with the current count before testing.
 * Return 1 when now - *stamp >= interval, otherwise 0. Only rearm == 1 writes
 * the current count after success; other values leave a nonzero stamp alone.
 * Rearming discards overshoot rather than advancing stamp by one interval.
 * No interval validation or signed-counter-wrap handling is supplied.
 * Original identity: exe:0x80012CB4. */
s32 HasVSyncIntervalElapsed(s32 interval, s32 *stamp, s32 rearm);

#endif
