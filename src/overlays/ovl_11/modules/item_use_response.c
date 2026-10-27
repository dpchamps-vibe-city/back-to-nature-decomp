/* Item-action response policy; flags take priority over the special IDs.
 * Original entry: ovl_11:800D736C. Protocol cuts: notes/file-groupings.md. */
#include "ovl11_item_use_response.h"

s32 Ovl11ClassifyItemUseResponse(s16 *itemIndex) {
    s16 id = *itemIndex;
    u16 flags = gItemDataTable[id].u0.flags;

    if (flags & 0x400) {
        return 4;
    }
    if (flags & 0x1800) {
        return 3;
    }
    if (flags & 0x40) {
        return 1;
    }
    if (flags & 0x100) {
        return 0;
    }
    if (id == 0x65 || id == 0x3F || id == 0x3A || id == 0x3E || id == 0x64) {
        return 5;
    }
    return 2;
}
