#ifndef OVL15_SAVE_SLOT_SELECTION_H
#define OVL15_SAVE_SLOT_SELECTION_H

#include "common.h"

/* Selection/preview part of modules/save_slot_transfer.c; transfer operations
 * are published by ovl15_save_slot_record_pool.h.
 * Two five-entry mark banks select resident (bank 0) and save-work (bank 1)
 * records. Update draws one frame and handles category/cursor input; nonzero
 * bank selects save-work. Returns -1 cancel, 0 continue, 1 confirm. It mutates
 * category, first-visible row, cursor, marks, UI scratch and sound.
 * Initialize invalidates all ten marks, clears their staged byte addresses
 * and the staging buffer: call BEFORE selection/staging, never between stage
 * and restore. Reset only resets selection state/count and its scratch; it
 * leaves marks, staged addresses and the staging buffer intact.
 */
s32 Ovl15UpdateSlotSelectionMenu(s16 bank);
void Ovl15InitializeSlotSelection(void);
void Ovl15ResetSlotSelection(void);

/* Draw both five-entry mark banks and their panels/labels; no input or result.
 * Resident records appear at x=40, save-work records at x=176 (pixels). Each
 * column starts at y=34 and advances 24 pixels only for occupied marks;
 * empty entries consume no vertical space. Record glyphs, sprite variants
 * and the quantity/25 meter are drawn through the active render context.
 * Marks/records are not modified. Occupied marks require valid pool indices
 * and a valid corresponding resident/save-work collection; indices are unchecked.
 */
void Ovl15DrawSlotBanks(void);

#endif
