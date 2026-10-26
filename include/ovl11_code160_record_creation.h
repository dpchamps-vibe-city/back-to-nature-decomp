#ifndef OVL11_CODE160_RECORD_CREATION_H
#define OVL11_CODE160_RECORD_CREATION_H

#include "common.h"

/*
 * Accepts codes 0x160..0x162, obtains a free 0xB8-byte tagged record and
 * initializes/places it in row-placement mode 1. Returns its address as a
 * signed word, or zero for an unsupported code/full pool. Both trailing
 * arguments are unread, retained for the existing caller ABI. The pool's
 * capacity and slot selection belong to ovl11_record_pair_slots.h.
 */
s32 Ovl11AllocateCode160Record(s32 code, s32 unused1, s32 unused2);

/*
 * Initializes destinationSlot with code 0x160, then copies sourceSlot's
 * +0x38/+0x3C/+0x40 words and +0x30/+0x22 halfwords, subtracting 200 from
 * the third word in raw coordinate units. Returns the destination address.
 * Indices are unchecked; self-copy is allowed. Initializer failure is
 * deliberately ignored, so an occupied destination still receives the copy.
 */
s8 *Ovl11InitCode160OffsetCopy(s32 sourceSlot, s32 destinationSlot);

#endif
