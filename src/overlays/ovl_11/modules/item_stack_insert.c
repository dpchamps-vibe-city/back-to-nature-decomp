/*
 * item_stack_insert — reconstructed translation unit.
 *
 * Count available capacity in 99-item stacks and insert a quantity into the
 * held/slot records, with a separate 64-record overflow fallback.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_item_stack_insert.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"

/* The original caller supplies two additional, unused zero arguments. */
s32 ovl_11_func_800E6EAC();

s16 Ovl11GetItemStackCapacity(s16 id, s32 mode) {
    s32 count;
    s32 total;
    s32 i;
    s16 *slot;

    if (mode == 0) {
        if (ovl_11_func_800E6EAC(0, 2, 0, 0) == 1) {
            return 99;
        }
    }
    total = 0;
    count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
    slot = (s16 *)&((struct Ovl11ItemInsertState *)D_8006C838)->held;
    if (slot[0] == 0) {
        total = 99;
    } else if (slot[0] == id) {
        total = 99 - slot[2];
    }
    /* Three halfwords per record; count is supplied by the saved exponent. */
    slot = (s16 *)&gItemStackSlotBase;
    for (i = 0; i < count; i++, slot += 3) {
        if (slot[0] == 0) {
            total += 99;
        } else if (slot[0] == id) {
            total += 99 - slot[2];
        }
    }
    return (s16)total;
}

s32 Ovl11InsertItemStack(s16 id, s16 amount, s32 mode) {
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
        insufficient = Ovl11GetItemStackCapacity(id, 1) < amount;
    }
    if (!insufficient) {
        count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
        /* fresh intentionally persists between slots. Once set, subsequent
         * occupied matching stacks also run the ID bookkeeping helper. */
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
                    Ovl11InstallSlotSprite(id);
                }
                Ovl11SetHalfwordTriplet(slot, id, 0, total);
            }
        }
        slot = (s16 *)&gItemStackSlotBase;
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
                        Ovl11InstallSlotSprite(id);
                    }
                    Ovl11SetHalfwordTriplet(slot, id, 0, total);
                }
            }
            if (amount <= 0) {
                break;
            }
        }
    } else {
        /* Overflow stores the entire quantity in one empty record, not a
         * sequence of 99-item stacks; it does not run ID bookkeeping. */
        empty = (s16 *)&gItemOverflowSlotBase;
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
