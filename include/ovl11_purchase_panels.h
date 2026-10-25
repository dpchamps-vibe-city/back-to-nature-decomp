#ifndef OVL11_PURCHASE_PANELS_H
#define OVL11_PURCHASE_PANELS_H

#include "common.h"

/* Overlay-11 interaction callbacks, selected by the common panel dispatcher.
 * Initialize once, then call the corresponding update once per interaction
 * tick until it returns 1 (0 means pending). Initializers return 1. Requires
 * text bank 1, a live message window, and valid item/price tables. All panels
 * reuse the global selection records, interaction steps/draw mode and message
 * state: only one may be active, and initialization overwrites that scratch.
 * Do not initialize one variant while another is being serviced.
 *
 * ovl_11:0x8011775C / 80117A40 / 80117BB4.
 * The time-dependent catalog chooses its list using hour 6 <= h < 18;
 * the other catalog has a fixed list. Update checks funds, debits the price,
 * and submits the selected ID to the active object at completion. It also
 * sets work flag 0x800000 while active and clears it on completion. */
s32 Ovl11InitTimeDependentCatalog(void);
s32 Ovl11InitFixedCatalog(void);
s32 Ovl11UpdateCatalogPurchasePanel(void);

/* ovl_11:0x80117F70 / 801180F0.
 * Initialize the first unset option among flags 0x3A..0x3E (-1 if all set),
 * using table index 4 as the fallback display/cost row. Update supports
 * per-unit purchases (stored resource units must remain below 1000) and an
 * option transaction charging both money and resource units. Successful
 * option payment sets flag 0x38 and invokes the completion service.
 * These names deliberately do not identify the resource or option as a
 * particular gameplay commodity/building. Units are the stored integer units. */
s32 Ovl11InitResourcePurchasePanel(void);
s32 Ovl11UpdateResourcePurchasePanel(void);

#endif
