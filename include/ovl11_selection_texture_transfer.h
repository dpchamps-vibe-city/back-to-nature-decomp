#ifndef OVL11_SELECTION_TEXTURE_TRANSFER_H
#define OVL11_SELECTION_TEXTURE_TRANSFER_H

#include "common.h"

/* Before the caller changes selection entries, inspect both pane categories
 * through F8404. Only a category-2/other crossing invokes texture movement.
 * The pointer IDs are read, not swapped here. releaseOutgoing==1 first
 * reconciles unused slots and releases the outgoing near-pane slot.
 * Caller must provide live ID pointers, slot/cache records, graphics heap
 * scratch at +4290/+4A90, and initialized VRAM. Uses synchronous GPU image
 * transfers, 8x32/8x64 images and 16x1 CLUTs; no allocation is performed.
 * The transfer is not transactional: missing/cache-full paths can return
 * after earlier image/slot changes. Not reentrant (shared RECT and scratch). */
void Ovl11ReorderSelectionTextures(u16 *firstId, s32 firstPane,
                                  u16 *secondId, s32 secondPane,
                                  s32 releaseOutgoing);

/* Snapshot 18 displayed IDs and release each far-table slot not represented
 * by that multiset. Duplicate IDs consume separate snapshot matches. */
void Ovl11ReleaseUnusedSelectionTextures(void);

#endif
