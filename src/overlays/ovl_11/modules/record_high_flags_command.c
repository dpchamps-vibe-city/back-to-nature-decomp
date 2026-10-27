/*
 * Replace a record's two high flag bits, ovl_11:800EB79C (opcode entry).
 * recordOrSlot and mode are narrowed to signed halfwords. Nonzero fromSlot
 * resolves the index from a FULL word value slot. At record+0x20, retain bits
 * 0..13; mode 0 adds 8000, mode 2 adds 4000, all other modes add neither.
 * Always return 1. Indices and initialized work/value slots are unchecked.
 * No C callers. Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"

s32 Ovl11SetRecordHighFlagsCommand(s32 recordOrSlot, s32 mode, s32 fromSlot) {
    s32 index;
    char *work;
    s32 recordIndex;
    char *record;
    u16 payload;

    index = (s16)recordOrSlot;
    mode = (s16)mode;
    if (fromSlot != 0) {
        recordIndex = gOvl11ValueSlots[index];
    } else {
        recordIndex = index;
    }
    work = (char *)&D_8006C838;
    record = work + recordIndex * 468;
    payload = *(u16 *)(record + 0x9A08) & 0x3FFF;
    *(u16 *)(record + 0x9A08) = payload;
    switch (mode) {
    case 0:
        *(u16 *)(record + 0x9A08) = payload | 0x8000;
        break;
    case 1:
        break;
    case 2:
        *(u16 *)(record + 0x9A08) = payload | 0x4000;
        break;
    }
    return 1;
}
