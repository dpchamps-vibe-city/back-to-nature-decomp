#ifndef OVL11_CAMERA_RECORD_SELECTION_H
#define OVL11_CAMERA_RECORD_SELECTION_H

#include "common.h"

/* ovl_11 must be resident. Resolve recordOrSlot through the shared value bank
 * when fromSlot != 0 (slot indices are unchecked). Selector 41 aliases -2:
 * the primary object's coordinates. Ordinary records are numbered 1..36;
 * each has stride 0x1D4 and integer X/s16-Y/Z coordinates at +0/+4/+8.
 *
 * moveMode == 0 only installs the selector; 2 moves without caching it;
 * other nonzero modes move and cache on success. -1 skips movement.
 * An already cached selection returns 1 without moving. speedCode is passed
 * to Ovl11MoveCameraTarget (narrowed to s16); see its step/completion contract.
 * Returns that move result, or 0 for an invalid moving selector, otherwise 1.
 *
 * Every call first sets work+C bit 400000, even on failure/cached return.
 * Installing -2 clears it; mode 2 does not. Uses the shared, non-reentrant
 * camera move/cache. Does not validate selectors when movement is skipped.
 * Original identity: ovl_11:800E880C. */
s32 Ovl11SelectCameraPositionRecord(s16 recordOrSlot, s16 fromSlot, s32 moveMode, s32 speedCode);

#endif
