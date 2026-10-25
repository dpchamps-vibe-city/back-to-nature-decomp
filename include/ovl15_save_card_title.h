#ifndef OVL15_SAVE_CARD_TITLE_H
#define OVL15_SAVE_CARD_TITLE_H

#include "common.h"

/* Replace the card header's 64-byte title with Shift-JIS text for save slot
 * (normally 0..2): file number slot+1, stored year+1, and stored season clamped
 * to 0..3. Returns the start of the title, as strcat does, not its end.
 * Requires a valid save-header record and single/two-digit numeric ranges;
 * there is no bounds check or numeric clamp. Shared scratch buffers make
 * this non-reentrant. Other card-header fields are untouched. */
s8 *Ovl15BuildSaveCardTitle(s16 slot);

#endif
