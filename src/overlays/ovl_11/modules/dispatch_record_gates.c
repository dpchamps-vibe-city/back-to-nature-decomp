/* Update the thirteen records in dispatch-pool slot 10 from season, area
 * variant and derived progress flags. Original ovl_11:800F5210–800F53BC.
 * Shared work is borrowed; no records are allocated or owned here.
 * Contract: ovl11_dispatch_record_flags.h. Evidence: notes/file-groupings.md. */
#include "ovl11_dispatch_record_flags.h"

void Ovl11UpdateList10RecordGates(s16 unusedSelector) {
    Ovl11DispatchGateCursor *record = (Ovl11DispatchGateCursor *)
        ((u8 *)((struct Ovl11DispatchGateWork *)D_8006C838)->records + 2);
    s16 kind;
    u16 flags;
    s32 index;

    /* The cursor begins at kind (+2), not the record ID (+0). Its stride
     * stays 0x18; this preserves the signed kind read and original base. */
    for (index = 0; index < 13; index++) {
        kind = record->kind;
        switch (kind) {
        case 0x64:
            if (!(((struct Ovl11DispatchGateWork *)D_8006C838)->derivedFlags & 0x20)) {
                record->flags &= 0xFFFE;
            } else {
                record->flags |= 1;
            }
            break;
        case 0x21:
            if (!(((struct Ovl11DispatchGateWork *)D_8006C838)->derivedFlags & 0x100)) {
                record->flags &= 0xFFFE;
            } else {
                record->flags |= 1;
            }
            break;
        case 0x128:
            if ((((struct Ovl11DispatchGateWork *)D_8006C838)->season != 0) &&
                (((struct Ovl11DispatchGateWork *)D_8006C838)->indexedVariant == 0)) {
                record->flags &= 0xFFFE;
            } else {
                record->flags |= 1;
            }
            break;
        case 0x129:
            if ((((struct Ovl11DispatchGateWork *)D_8006C838)->season != 1) &&
                (((struct Ovl11DispatchGateWork *)D_8006C838)->indexedVariant == 0)) {
                record->flags &= 0xFFFE;
            } else {
                record->flags |= 1;
            }
            break;
        case 0x12A:
            if ((((struct Ovl11DispatchGateWork *)D_8006C838)->season != 2) &&
                (((struct Ovl11DispatchGateWork *)D_8006C838)->indexedVariant == 0)) {
                record->flags &= 0xFFFE;
            } else {
                record->flags |= 1;
            }
            break;
        }
        flags = record->flags;
        if (flags & 0x8000) {
            if (((struct Ovl11DispatchGateWork *)D_8006C838)->derivedFlags & 0x40) {
                record->kind = 0x6D;
                record->flags = flags | 1;
            } else if (((struct Ovl11DispatchGateWork *)D_8006C838)->derivedFlags & 0x80) {
                record->kind = 0x6E;
                record->flags = flags | 1;
            } else {
                record->flags = flags & 0xFFFE;
            }
        }
        record++;
    }
}
