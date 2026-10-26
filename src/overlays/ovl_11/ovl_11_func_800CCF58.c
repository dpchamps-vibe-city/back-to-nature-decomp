#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_requests.h"

void ovl_11_func_800CCF58(Ovl11C7010Object *arg0) {
    s16 itemIndex;
    s32 flags;
    s32 *flagWord;

    itemIndex = arg0->itemIndex;
    if (itemIndex == 0) {
        arg0->unk6C &= 0xE7FF7FFF;
        Ovl11ResetPrimaryObjectRequest();
    } else {
        flagWord = &arg0->unk6C;
        flags = *flagWord & 0xE7FFFFFF;
        *flagWord = flags;
        if (D_8006C858[itemIndex].u0.flags & 0x180) {
            arg0->unk6C = flags | 0x08000000;
        } else {
            arg0->unk6C = flags | 0x10000000;
        }
        arg0->unk6C |= 0x8000;
    }
}
