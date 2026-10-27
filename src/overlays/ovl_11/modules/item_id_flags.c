/*
 * item_id_flags — reconstructed translation unit.
 *
 * Add item-code-specific bits to a record flag word at +0x34, preserving all
 * existing bits. Shared by allocation and item transitions; it owns no state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_item_id_flags.h"

void Ovl11ApplyItemIdFlags(void *record) {
    u16 itemId = *(u16 *)record;
    u32 *flags = (u32 *)((char *)record + 0x34);

    switch (itemId) {
    case 0xA1:
    case 0x108:
    case 0x10B:
    case 0x10C:
    case 0x10D:
    case 0x10E:
    case 0x10F:
    case 0x110:
    case 0x112:
    case 0xB2:
    case 0xB3:
    case 0xB4:
    case 0xB5:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBD:
    case 0xBE:
    case 0xBF:
    case 0xC0:
    case 0xC1:
    case 0xC3:
    case 0xC4:
    case 0x159:
    case 0x15B:
    case 0x165:
        break;
    case 0x162:
    case 0x163:
        *flags |= 0x80000000;
        return;
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
    case 0x117:
    case 0x118:
    case 0x119:
    case 0x11A:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x156:
    case 0x157:
        *flags |= 0x1000;
        break;
    }
}
