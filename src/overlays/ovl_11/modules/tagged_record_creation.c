/*
 * Creation facades for the shared 0xB8-byte tagged-record pool. Both use the
 * 0x164/0x165 initializer; the indexed-copy path deliberately ignores its
 * failure result. No ownership or private linkage is inferred for the pool.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_tagged_record_creation.h"

s32 *ovl_11_func_800D0C34(void);
s32 ovl_11_func_800D0D7C(u8 *record);
s32 ovl_11_func_800E0D3C(UnkStruct800E109C *record, s32 code);
void ovl_11_func_800D075C(s32 recordAddress, s16 slot, s32 mode);

s32 Ovl11AllocateTaggedRecord(s32 code, s32 unused1, s32 unused2) {
    s32 recordAddress;
    s32 slot;

    if (((u32)(code + -0x164)) < ((u32)2)) {
        recordAddress = (s32)ovl_11_func_800D0C34();
        if (recordAddress == 0) {
            return 0;
        } else {
            slot = ovl_11_func_800D0D7C((u8 *)recordAddress);
            ovl_11_func_800E0D3C((UnkStruct800E109C *)recordAddress, code);
            ovl_11_func_800D075C(recordAddress, ((s16)slot), 1);
            return recordAddress;
        }
    } else {
        return 0;
    }
}

UnkStruct800E109C *Ovl11InitOffsetRecordCopy(s32 sourceSlot, s32 destinationSlot) {
    UnkStruct800E109C *destination;
    void *source;

    destination = (UnkStruct800E109C *) ((u8 *) &gTaggedRecordBase + (destinationSlot * 0xB8));
    ovl_11_func_800E0D3C(destination, 0x164);
    source = (void *) ((u8 *) &gTaggedRecordBase + (sourceSlot * 0xB8));
    *(s32 *) ((u8 *) destination + 0x38) = *(s32 *) ((u8 *) source + 0x38);
    *(s32 *) ((u8 *) destination + 0x3C) = *(s32 *) ((u8 *) source + 0x3C);
    *(s32 *) ((u8 *) destination + 0x40) = *(s32 *) ((u8 *) source + 0x40) - 0xC8;
    *(u16 *) ((u8 *) destination + 0x30) = *(u16 *) ((u8 *) source + 0x30);
    *(u16 *) ((u8 *) destination + 0x22) = *(u16 *) ((u8 *) source + 0x22);
    return destination;
}
