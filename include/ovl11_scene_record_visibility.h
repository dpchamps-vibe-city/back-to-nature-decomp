#ifndef OVL11_SCENE_RECORD_VISIBILITY_H
#define OVL11_SCENE_RECORD_VISIBILITY_H

/* ovl_11 must be resident. These calls change record flags only; the scene
 * renderer later skips records with bit 0x1000 set. No GPU work is submitted.
 * The saved pair is shared state, not reentrant or an owning allocation. */

/* Scan the fifteen scene records for flag 0x10, selecting a slot by bit 0.
 * Stop at the first slot-1 record; the LAST preceding slot-0 record wins.
 * Both slots MUST exist: setup dereferences them without checking. Initially
 * show slot 0 and hide slot 1. Original identity: ovl_11:0x800DDC64. */
void Ovl11InitSceneRecordVisibilityPair(void);

/* After setup, show slot 1 if game flag 0x72 is 1, otherwise show slot 0.
 * Leave every other record bit intact. A null slot 0 makes this a no-op;
 * slot 1 is not checked. Original identity: ovl_11:0x800DDD18. */
void Ovl11UpdateSceneRecordVisibilityPair(void);

#endif
