#ifndef OVL11_GROUPED_VALUE_MAPPING_H
#define OVL11_GROUPED_VALUE_MAPPING_H

#include "common.h"

/* Pure s16 maps; out-of-range inputs return zero (also a valid group).
 * For v=1..25, g=(v-1)/5:
 *   mapped code = 0x48 + 20*g + (v-(5*g+1)); group index = g.
 * For v=26..29: mapped code = 0xB6 + 4*(v-26); group index = v-26.
 * These are code/index values, not coordinates or byte offsets. */
s16 Ovl11MapGroupedValueCode(s16 value);
s16 Ovl11GetGroupedValueIndex(s16 value);

#endif
