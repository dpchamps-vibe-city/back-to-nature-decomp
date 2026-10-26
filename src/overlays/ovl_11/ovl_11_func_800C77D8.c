#include "common.h"
#include "game_types.h"
#include "ovl11_item_action_gate.h"

s32 ovl_11_func_800C77D8(Ovl11C7010Object *arg0, u16 arg1) {
    if (!(arg1 & 0x80)) {
        return 0;
    }
    if (!(arg0->unk6C & 0x10000000)) {
        return 0;
    }
    if (arg0->itemIndex != 0 && Ovl11TryBeginItemAction(1) != 0) {
        return 1;
    }
    return 0;
}
