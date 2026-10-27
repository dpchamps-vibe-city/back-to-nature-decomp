#ifndef OVL11_FLAGGED_CODE109_RECORDS_H
#define OVL11_FLAGGED_CODE109_RECORDS_H

#include "common.h"

/* Only control's low halfword matters. Zero attempts five allocations of
 * ID 0x109, independently skipping failures; each successful record gains
 * flag 0x40000 at +0x34 and an opaque 18-byte bank-1 payload at +4.
 * Repeated loads can allocate duplicates. The text bank must be available.
 *
 * Nonzero instead clears EVERY marked record in the ten-slot small-record
 * pool, regardless of ID. It does not merely undo the last five allocations.
 * Neither path reports allocation failure: both return 1. */
s32 Ovl11LoadOrClearCode109Records(s32 control);

#endif
