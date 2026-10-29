#ifndef OVL11_PRICE_OPTION_ROWS_H
#define OVL11_PRICE_OPTION_ROWS_H

#include "common.h"

/* Record initializers for trade/quantity-purchase interactions; no inputs,
 * return 1 (handled). Both refresh derived progress flags, set quantity=1,
 * obtain the unit price (item 0x68 / 0x69 respectively), initialize the shared
 * primary selection record and clear gOvl11InteractionDrawMode. Trade builds
 * nine rows, including progress- and item-slot-gated entries; quantity builds
 * four, with its last text gated by progress bit 14. Unavailable row texts are
 * NULL; their value/type/auxiliary halfwords are still written.
 * Text addresses reference bank 1; no allocation or drawing happens here.
 * Shared quantity/selection records are overwritten and must not be in use by
 * another interaction. Additional callbacks are exposed through the table,
 * not as part of this C-call interface.
 */
s32 Ovl11BuildTradePriceRows(void);
s32 Ovl11BuildQuantityPriceRows(void);

#endif
