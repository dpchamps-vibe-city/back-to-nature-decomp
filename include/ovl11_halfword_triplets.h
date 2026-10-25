#ifndef OVL11_HALFWORD_TRIPLETS_H
#define OVL11_HALFWORD_TRIPLETS_H

#include "common.h"
#include "game_types.h"

/* Add delta to the last halfword modulo 65536. Return 1 only if the signed
 * truncated result is zero. A negative truncated result is stored as -1 and
 * returns 0; positive results are stored unchanged. Leading four bytes stay
 * untouched. The tail is a caller-defined value, not necessarily a coordinate. */
s32 Ovl11AddTripletTail(Ovl11TripletTailView *record, s16 delta);

/* Six-byte, halfword-aligned record operations. No validation or state.
 * Copy performs three forward assignments, preserving their overlap behavior. */
void Ovl11ClearHalfwordTriplet(s16 *values);
void Ovl11SetHalfwordTriplet(s16 *values, s16 first, s16 second, s16 third);
void Ovl11CopyHalfwordTriplet(u16 *destination, u16 *source);

#endif
