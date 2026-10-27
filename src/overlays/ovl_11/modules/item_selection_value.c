/* Fixed item selection values; no general item-table/price lookup.
 * Original entry: ovl_11:800D740C. Boundaries: notes/file-groupings.md. */
#include "ovl11_item_selection_value.h"

s32 Ovl11LookupItemSelectionValue(u16 itemId) {
    s32 result = 0;

    switch (itemId) {
    case 0x67:
        result = 0x3E9;
        break;
    case 0x185:
        result = 0x61;
        break;
    case 0xF7:
        result = 0x2A;
        break;
    case 0xF8:
        result = 0x23;
        break;
    case 0xF9:
        result = 0x21;
        break;
    case 0x9E:
        result = 0x15;
        break;
    case 0x9C:
        result = 0xF;
        break;
    case 0x5C:
        result = 0xC;
        break;
    case 0x97:
        result = 7;
        break;
    case 0x60:
        result = 3;
        break;
    }
    return result;
}
