#ifndef OVL11_MENU_GRID_VIEW_H
#define OVL11_MENU_GRID_VIEW_H

#include "common.h"

/* Interaction-menu view. Ovl11ResetInteractionSequence initializes shared
 * cursor, selection, page, message and animation state. Initialize pad 0,
 * render layers, text banks, slot caches and shared sprite source first.
 * Coordinates in the private 17-row navigation table are screen pixels;
 * records are THREE signed halfwords {id, detail, quantity} (six bytes).
 * Slot 0 is the selected record, 1..8 auxiliary records, 9..16 an eight-cell
 * page. gMenuGridLayoutIndex must be 0..2. There are no bounds/null guards.
 * This view does not transfer, compact or otherwise modify item records.
 */

/* Read held pad buttons only while message status is final; follow table
 * links, wrap pages and draw cursor/selection markers on active render layers.
 * slotCount/8 is the page count and MUST be nonzero (normally slotCount=64).
 * Navigation/page changes require message state FF. Selected-slot FF means
 * none; a paged selection marker is shown only on its selected page. */
void Ovl11UpdateMenuGridNavigation(s32 slotCount);

/* With message state != FF, resolve slot/page and request its message.
 * ID A4 uses the record's mapped item ID +1000. Message status 2 closes it
 * and restores FF. First two arguments are unused but retain their ABI. */
void Ovl11UpdateMenuGridMessage(s32 textDisplay, s32 panelDisplay, s32 slot, s32 page);

/* Render one slot, or count consecutive records starting at firstSlot.
 * ID zero draws empty; slots <9 use the sprite cache, later slots the texture
 * cache. Type 2 always shows quantity; type 0 shows it only at level 2.
 * orderingTable addresses the display layer; quantity uses active layer +58. */
void Ovl11DrawMenuGridSlot(s32 *orderingTable, s32 slot, s16 *record);
void Ovl11DrawMenuGridSlots(s32 *orderingTable, s32 firstSlot, s32 count, s16 *records);

/* Draw one-based current page / slotCount/8 through shared 12-halfword
 * scratch text. Non-reentrant; strings use FFFF termination. */
void Ovl11DrawMenuGridPageCount(s32 textDisplay, s32 slotCount);

/* Draw three panels and the two caller-supplied text strings (addresses use
 * the existing s32 ABI), plus two animated indicators. Each call advances
 * their four-frame animation once per nine calls, not per elapsed time.
 * The shared sprite source is left on animation C and the displayed frame. */
void Ovl11DrawMenuGridPanels(u32 *panelOrderingTable, s32 textDisplay,
                           s32 topTextAddress, s32 bottomTextAddress);

#endif
