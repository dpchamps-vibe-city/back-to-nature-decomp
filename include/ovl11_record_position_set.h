#ifndef OVL11_RECORD_POSITION_SET_H
#define OVL11_RECORD_POSITION_SET_H

#include "common.h"
#include "game_types.h"

/* ovl_11 must be resident. Assign the record's signed-word XYZ position at
 * +58/+5C/+60, set flag 2000 and clear flag 4000 at +34 (hex offsets/bits).
 * No coordinate conversion, interpolation, collision test or allocation is
 * performed. Other fields and flag bits are retained; there is no result.
 * The input is the canonical four-word position passed by value: its fourth
 * word is carried by the ABI but is not read. Coordinate units are unchanged.
 * Original identity: ovl_11:800D05D0. */
void Ovl11SetRecordPosition(Ovl11SetFieldsView *record, Ovl11PaddedVec3 position);

#endif
