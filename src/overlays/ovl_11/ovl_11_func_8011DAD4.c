#include "common.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"

s16 ovl_11_func_8011D9DC(s16, s32);
s32 ovl_11_func_800D6A94(s16);

s32 ovl_11_func_8011DAD4(s16 id, s16 amount, s32 mode) {
    s32 i;
    s32 count;
    s32 capacity;
    s32 total;
    s32 fresh;
    s32 insufficient;
    s16 *slot;
    s16 *empty;

    if (mode == 1) {
        insufficient = 0;
    } else {
        insufficient = ovl_11_func_8011D9DC(id, 1) < amount;
    }
    if (!insufficient) {
        count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
        fresh = 0;
        slot = (s16 *)((u8 *)D_8006C838 + 0x524C);
        capacity = 0;
        if (slot[0] == 0) {
            capacity = 99;
        } else if (slot[0] == id) {
            capacity = 99 - slot[2];
        }
        if (slot[0] == id || (u16)slot[0] == 0) {
            if (amount <= capacity) {
                if ((u16)slot[0] == 0) {
                    slot[2] = 0;
                    fresh = 1;
                }
                total = slot[2] + amount;
                amount = 0;
            } else {
                total = 99;
                amount -= capacity;
            }
            if (total != 0) {
                if (fresh == 1) {
                    ovl_11_func_800D6A94(id);
                }
                Ovl11SetHalfwordTriplet(slot, id, 0, total);
            }
        }
        slot = (s16 *)&D_80071A90;
        for (i = 0; i < count; i++, slot += 3) {
            capacity = 0;
            if (slot[0] == 0) {
                capacity = 99;
                fresh = 1;
            } else if (slot[0] == id) {
                capacity = 99 - slot[2];
            }
            if (slot[0] == id || (u16)slot[0] == 0) {
                if (amount <= capacity) {
                    if ((u16)slot[0] == 0) {
                        slot[2] = 0;
                    }
                    total = slot[2] + amount;
                    amount = 0;
                } else {
                    total = 99;
                    amount -= capacity;
                }
                if (total != 0) {
                    if (fresh == 1) {
                        ovl_11_func_800D6A94(id);
                    }
                    Ovl11SetHalfwordTriplet(slot, id, 0, total);
                }
            }
            if (amount <= 0) {
                break;
            }
        }
    } else {
        empty = (s16 *)&D_80070EC2;
        for (i = 0; i < 64; i++, empty += 3) {
            if (empty[0] == 0) {
                Ovl11SetHalfwordTriplet(empty, id, 0, amount);
                amount = 0;
                break;
            }
        }
    }
    return amount;
}
