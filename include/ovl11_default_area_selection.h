#ifndef OVL11_DEFAULT_AREA_SELECTION_H
#define OVL11_DEFAULT_AREA_SELECTION_H

#include "common.h"

/* Read resident base/variant50/60/70/80; return an area-selection ID without
 * changing state. Primary variants 0/1 add the variant70/60 zero-or-nonzero
 * offset, respectively; primary 2 indexes variant80 (required to be 0 or 1).
 * Other primary values return the base alone. Offsets are signed halfwords;
 * no clamping or calendar remapping is performed here. */
s32 Ovl11ResolveDefaultAreaSelection(void);

#endif
