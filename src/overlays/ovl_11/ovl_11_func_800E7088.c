#include "common.h"
#include "game_types.h"

/* Overlay name for the matched pow_int at 0x8001F250. */
s32 pow_int(s32 arg0, s32 arg1);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800D6A94(s16 arg0);
void ovl_11_func_800D7338(s16 *arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_11_func_800C4F44(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E7088(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 id;
    s16 variant;
    s16 *selected;
    Ovl11ItemState *slot;
    s32 overflow;
    s32 index;
    s32 count;
    u32 i;
    s32 mode;
    s32 result;

    mode = 2;
    if (arg2 != 0) {
        mode = 1;
    }
    if (!ovl_11_func_800E6EE0(0, 0, mode, 0)) {
        return 0;
    }
    if (arg1 != 0) {
        id = *(s16 *)&D_80129560[arg0];
    } else {
        id = arg0;
    }
    selected = 0;
    overflow = 0;
    index = -1;
    if (((struct Ovl11ItemInsertState *)D_8006C838)->held.id == 0) {
        selected = (s16 *)&((struct Ovl11ItemInsertState *)D_8006C838)->held;
    } else {
        count = pow_int(2, ((struct Ovl11ItemInsertState *)D_8006C838)->exponent + 1);
        slot = ((struct Ovl11ItemInsertState *)D_8006C838)->slots;
        for (i = 0, index = 0; (s32)i < count; i++, slot++, index++) {
            if (slot->id == 0) {
                selected = (s16 *)slot;
                break;
            }
        }
        if (selected == 0) {
            slot = (Ovl11ItemState *)&D_80070EC2;
            for (i = 0; i < 64; i++, slot++) {
                if (slot->id == 0) {
                    selected = (s16 *)slot;
                    overflow = 1;
                    break;
                }
            }
        }
    }
    result = 0;
    if (selected != 0) {
        variant = 0;
        if ((u16)(id - 1) < 25U) {
            variant = (id - 1) % 5;
        }
        if (overflow == 0) {
            ovl_11_func_800D6A94(id);
        }
        ovl_11_func_800D7338(selected, id, variant, 1);
        if (overflow == 0 && arg3 != 0) {
            ovl_11_func_800C4F44((Recon_ovl_11_func_800C7270_A0View *)&D_80071A00,
                               index != -1, index, arg3 == 1 ? 10 : 60);
        }
        result = 1;
    }
    return result;
}
