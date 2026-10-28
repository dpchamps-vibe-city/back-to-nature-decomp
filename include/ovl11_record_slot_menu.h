#ifndef OVL11_RECORD_SLOT_MENU_H
#define OVL11_RECORD_SLOT_MENU_H

#include "common.h"

/* Complete item-slot screen. Ovl11ResetInteractionSequence initializes its
 * cursor, selection, page, message and animation state. Pad 0, render layers,
 * text banks, slot/texture caches and the shared sprite source must be ready.
 * gMenuGridLayoutIndex must be 0..2; no bounds/null checks are performed.
 *
 * Service one frame: compose three panels, process pressed-button record
 * operations, navigate with repeatButtons, draw records/cursors and service
 * the selected-record message. Interaction phases 0, 1 and 2 select the three
 * 64-slot banks; 0xFE requests exit. Message state FF permits navigation and
 * selection; other states service the record message until status 2 closes it.
 * Stack merges cap the destination at 99 and leave excess in the source.
 *
 * Drawing uses screen-pixel coordinates and the active context's layers
 * +50..+68. Panel draws advance the two four-frame indicators once per nine
 * calls, not elapsed time. The shared sprite is left on animation C. Page
 * text uses shared FFFF-terminated scratch; the screen is non-reentrant.
 * Slot operations also reorder selection textures. No allocation or rollback
 * is supplied here; callers must preserve the initialized storage lifetime. */
void Ovl11UpdateMenuGridScreen(void);

/* Resolve a menu position to a mutable three-halfword record (ID, attribute,
 * count). Slot 0 is the selected record, slots 1..8 the snapshot row, and
 * slots >=9 the eight-per-page bank. No bounds checking is performed. */
s16 *Ovl11GetItemSlotRecord(s32 slot, s32 page);

/* Classify a slot as selected (0), snapshot row (1), or paged bank (2).
 * Callers supply nonnegative slot numbers. */
s32 Ovl11GetItemSlotLevel(s32 slot);

#endif
