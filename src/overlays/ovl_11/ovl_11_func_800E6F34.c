#include "common.h"
#include "integer_power.h"

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800D7338(s16 *arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_800CB540(u16 *arg0);
s32 ovl_11_func_800D6A94(s16 arg0);

s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 item[3];
    u16 id;
    s32 count;
    s16 i;
    Ovl11ItemState *slot;

    if (arg1 != 0) {
        id = *(u16 *)&gOvl11ValueSlots[arg0];
    } else {
        id = arg0;
    }
    count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
    if (arg2 == 0) {
        if (!ovl_11_func_800E6EE0(0, 1, 0, 0)) {
            return 0;
        }
        ovl_11_func_800D7338(item, id, 0, 1);
        ovl_11_func_800CB540((u16 *)item);
        return 1;
    } else {
        if (!ovl_11_func_800E6EE0(0, 1, -1, 0)) {
            return 0;
        }
        slot = ((struct Ovl11ItemGridState *)D_8006C838)->slots;
        for (i = 0; i < count && slot->id != 0; i++, slot++) {
        }
        ovl_11_func_800D6A94(id);
        ovl_11_func_800D7338((s16 *)slot, id, 0, 1);
        return 1;
    }
}
