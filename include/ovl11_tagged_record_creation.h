#ifndef OVL11_TAGGED_RECORD_CREATION_H
#define OVL11_TAGGED_RECORD_CREATION_H

#include "common.h"
#include "game_types.h"

/*
 * Accepts only codes 0x164/0x165; obtains a free 0xB8 record, initializes it and
 * places it through the slot-indexed mode-1 placement service. Returns its
 * address as a signed word (the existing caller ABI), or zero for an unsupported
 * code/full pool. The allocator searches 10 or 20 slots according to 80070D08.
 * Both trailing arguments are retained but unread. Consumes no RNG here.
 */
s32 Ovl11AllocateTaggedRecord(s32 code, s32 unused1, s32 unused2);

/*
 * Initializes destinationSlot with code 0x164, then copies sourceSlot's three
 * +0x38/+0x3C/+0x40 words and +0x30/+0x22 halfwords, subtracting 200 from the
 * third word (raw coordinate units). Returns the destination, not a status.
 * Slots are unchecked; source and destination may coincide. Initialization's
 * failure is ignored: an already-initialized destination still receives the
 * copies. Shared pool storage and all field widths remain unchanged.
 */
UnkStruct800E109C *Ovl11InitOffsetRecordCopy(s32 sourceSlot, s32 destinationSlot);

#endif
