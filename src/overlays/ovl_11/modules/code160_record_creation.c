/*
 * Creation facades for codes 0x160..0x162 in the shared tagged-record pool.
 * Allocation and indexed-copy use the same specialized initializer; the
 * copy path intentionally ignores its failure result. Pool ownership and
 * helper linkage are unchanged. Public contracts: ovl11_code160_record_creation.h.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_code160_record_creation.h"
#include "ovl11_record_pair_slots.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"

s32 Ovl11AllocateCode160Record(s32 code, s32 unused1, s32 unused2) {
    s32 recordAddress;
    s32 slot;

    if ((u32)(code - 0x160) < 3U) {
        recordAddress = (s32)Ovl11FindFreeTaggedRecord();
        if (recordAddress == 0) {
            return 0;
        } else {
            slot = Ovl11GetTaggedRecordIndex((u8 *)recordAddress);
            Ovl11InitCode160Record((char *)recordAddress, code);
            Ovl11ApplyRecordRowPlacement(recordAddress, (s16)slot, 1);
            return recordAddress;
        }
    } else {
        return 0;
    }
}

s8 *Ovl11InitCode160OffsetCopy(s32 sourceSlot, s32 destinationSlot) {
    u8 *destination;
    u8 *source;

    destination = (u8 *)&gTaggedRecordBase + destinationSlot * 0xB8;
    source = (u8 *)&gTaggedRecordBase + sourceSlot * 0xB8;
    Ovl11InitCode160Record((char *)destination, 0x160);
    *(s32 *)(destination + 0x38) = *(s32 *)(source + 0x38);
    *(s32 *)(destination + 0x3C) = *(s32 *)(source + 0x3C);
    *(s32 *)(destination + 0x40) = *(s32 *)(source + 0x40) - 200;
    *(u16 *)(destination + 0x30) = *(u16 *)(source + 0x30);
    *(u16 *)(destination + 0x22) = *(u16 *)(source + 0x22);
    return (s8 *)destination;
}
