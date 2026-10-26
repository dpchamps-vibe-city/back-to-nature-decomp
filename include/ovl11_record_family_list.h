#ifndef OVL11_RECORD_FAMILY_LIST_H
#define OVL11_RECORD_FAMILY_LIST_H

#include "common.h"

/* Resident ovl_11 with initialized tagged-record families, text bank, sprite
 * data and active render context required. Draw a titled browser of at most
 * four rows, 24 pixels apart. requestedKind is clamped to 0..2; filter is
 * forwarded unchanged to the shared family scanner. Cursor/scroll are shared
 * signed halfword indices retained between calls; reset when changing lists.
 *
 * inputMode == -1: clear cursor/scroll and return -2, without drawing.
 * inputMode == 0: process repeated up/down (wrap cursor), cancel/confirm,
 *                 play selection sounds; return -1 while waiting, -2 on
 *                 cancel or the scanner's selected record index on confirm.
 * Other modes: draw without input and immediately return the current record
 *              index. No nonempty-list or scratch-capacity check is added.
 * Mutates frame packets and the shared text-format scratch. Names/row values
 * retain their original signedness; the gauge casts its u16 value to s16
 * before dividing by 25, then draws that many segments at an 8-pixel pitch. */
s32 Ovl11DrawAndProcessRecordFamilyList(s16 requestedKind, s16 filter, s16 inputMode);

#endif
