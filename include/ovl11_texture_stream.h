#ifndef OVL11_TEXTURE_STREAM_H
#define OVL11_TEXTURE_STREAM_H

#include "common.h"

/* Overlay-11 batch texture loader. Requires an initialized archive index,
 * graphics-heap scratch at +0x4290 (0x28000 bytes), populated request lists,
 * and enough free slots in the shared 72-entry texture cache. Uses the single
 * CD byte-read channel; do not interleave other reads or scratch users.
 *
 * ovl_11:0x800F77F0. Call once per service update until zero. Returns 1 while
 * reading/installing; zero means the cursor passed the last batch (first ID
 * < 0x13F). requestMode 0 selects the +468A list, 1 the +450A list plus eight
 * extra IDs, and 2 the +480A list. Other modes are not a safe public input:
 * the retail fallback count is not bounded by that list's capacity.
 *
 * Each batch reads 80 records of 2048 bytes at archive-0 offset
 * 0x03C19000 + 2048*firstId. Installation writes cache metadata and VRAM,
 * including CLUTs. Exhausting the cache leaves partial results and reports
 * busy indefinitely through this API. Completion is NOT latched: stop
 * calling on zero or the next call will start another read. Non-reentrant. */
s32 Ovl11StepTextureStream(s32 requestMode);

/* ovl_11:0x800F77A8. -1 resets the encoded cursor and returns zero; it does
 * not cancel a CD transfer or clear the cache. Every other argument steps
 * mode 1 (the argument itself is not a mode), returning the result as s16. */
s32 Ovl11ServiceTextureStream(s16 resetOrStep);

#endif
