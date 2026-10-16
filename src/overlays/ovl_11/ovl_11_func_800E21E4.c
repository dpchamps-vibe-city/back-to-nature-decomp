#include "common.h"
#include "game_types.h"

/* Overlay import of Rand; return signedness is established by src/Rand.c. */
u32 func_80012A34(s32 arg0);
s32 ovl_11_func_800E2934(void *arg0);
s32 ovl_11_func_800E2A30(s16 *arg0);
void ovl_11_func_800E2BE8(s32 arg0);
s32 ovl_11_func_800E2654(E21E4Position *src, E21E4Position *dst);
/* The +38 aggregate occupies four argument words (a1-a3 and stack +10).
 * F43CC's current definition spells those same words as four byte blocks. */
void ovl_11_func_800F43CC(s32 arg0, E21E4Position position, u16 arg5, s32 arg6);
void ovl_11_func_800CF2C8(s32 *arg0);
void ovl_11_func_80107DD0(s16 *arg0);

s32 ovl_11_func_800E21E4(E21E4Object *arg0) {
    s32 index;
    s32 i;
    s32 energy;
    s32 temp;
    s32 out;
    s32 selection;
    E21E4Position position;

    if (arg0->type == 0 && !(arg0->flags & 0x2000000)) {
        return -1;
    }
    if (arg0->type != 0x109) {
        if (arg0->type == 0x10A && (arg0->flags & 0x2000000)) {
            return 0;
        }
        arg0->timer++;
        ovl_11_func_800E2A30((s16 *)arg0);
        ovl_11_func_800E2BE8((s32)arg0);
        return 0;
    }
    index = ovl_11_func_800E2934(arg0);
    if (arg0->kind == 4 && !(arg0->flags & 1)) {
        for (i = 0; i < 10; i++) {
            if (((E21E4State *)D_8006C838)->pending[i] != 0) {
                arg0->flags |= 1;
                ((E21E4State *)D_8006C838)->pending[i] = 0;
                break;
            }
        }
    }
    ovl_11_func_800E2BE8((s32)arg0);
    if (arg0->flags & 0x10) {
        if ((s16)arg0->energy < 100) {
            selection = 0x122;
        } else if ((s16)arg0->energy < 200) {
            selection = 0x123;
        } else if (arg0->flags & 0x40) {
            selection = 0xE8;
        } else {
            selection = 0x124;
        }
        if (arg0->kind == 4) {
            ovl_11_func_800E2654(&arg0->position, &position);
        } else {
            position = arg0->position;
        }
        ovl_11_func_800F43CC(selection, position, arg0->kind, index);
    }
    if (arg0->flags & 1) {
        s32 checkedEnergy;
        energy = arg0->energy + 10;
        arg0->energy = energy;
        checkedEnergy = (s16)energy;
        temp = energy;
        if (checkedEnergy >= 256) {
            temp = 255;
        }
        arg0->energy = temp;
        arg0->flags |= 0x10;
    } else {
        s32 checkedEnergy;
        energy = arg0->energy - 20;
        arg0->energy = energy;
        checkedEnergy = (s16)energy;
        temp = energy;
        if (checkedEnergy < 0) {
            temp = 0;
        }
        arg0->energy = temp;
        arg0->flags &= ~0x10;
    }
    if (!(arg0->flags & 0x100)) {
        temp = arg0->counter;
        if (temp != 0) {
            temp++;
            arg0->counter = temp;
            if ((u16)temp >= 32) {
                temp = 31;
            }
            arg0->counter = temp;
            if (arg0->counter >= 3 && !(arg0->flags & 0x40000) &&
                (s32)func_80012A34(100) <= arg0->counter * 10) {
                arg0->flags |= 0x100;
            }
        } else if ((s16)arg0->energy <= 0 && func_80012A34(100) < 11) {
            arg0->counter++;
        }
    }
    if (!(arg0->flags & 0x100)) {
        if (!(arg0->flags & 0x40000)) {
            if (arg0->age < 3) {
                selection = 0;
            } else if (arg0->age == 3) {
                selection = 25;
            } else if (arg0->age == 4) {
                selection = 75;
            } else {
                selection = 100;
            }
            if (selection != 0 && func_80012A34(100) <= (u32)selection) {
                arg0->flags |= 0x80100;
            }
        }
    }
    temp = arg0->timer;
    temp++;
    arg0->timer = temp;
    out = temp;
    if ((u16)out >= 32) {
        out = 31;
    }
    arg0->timer = out;
    ovl_11_func_800E2A30((s16 *)arg0);
    if (arg0->day == ((E21E4State *)D_8006C838)->day &&
        arg0->mode == ((E21E4State *)D_8006C838)->mode) {
        arg0->age++;
    }
    if (!(arg0->flags & 1)) {
        ((E21E4State *)D_8006C838)->flags |= 0x8000000;
    }
    ovl_11_func_800CF2C8((s32 *)arg0);
    arg0->fieldAC = 0;
    arg0->flags &= ~1;
    arg0->flags &= ~0x2000;
    ovl_11_func_80107DD0(arg0->reset);
    return 0;
}
