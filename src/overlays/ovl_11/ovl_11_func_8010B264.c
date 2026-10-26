#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

s32 ovl_11_func_8010B830(Ovl11B264Object *arg0);

s32 ovl_11_func_8010B264(Ovl11B264Object *arg0) {
    s16 value;
    s32 flags;
    s32 amount;
    s32 temp;
    s32 temp2;
    s32 temp3;
    s32 temp4;
    s32 temp5;
    s32 out;
    s32 out2;
    s32 out3;
    s32 out4;
    s32 out5;

    if (arg0->field_0 == 0) {
        return -1;
    }
    flags = arg0->field_34;
    if (flags & 0x400) {
        return -1;
    }
    if (flags & 2) {
        temp = arg0->field_16 + 2;
        arg0->field_16 = temp;
        if ((s16) temp >= 0x100) {
            out = 0xFF;
        } else {
            out = temp;
        }
        arg0->field_16 = out;
    }
    if (arg0->field_34 & 4) {
        temp2 = arg0->field_16 + 2;
        arg0->field_16 = temp2;
        if ((s16) temp2 >= 0x100) {
            out2 = 0xFF;
        } else {
            out2 = temp2;
        }
        arg0->field_16 = out2;
    }
    if (!(arg0->field_34 & 0x26)) {
        temp3 = arg0->field_16 - 1;
        arg0->field_16 = temp3;
        if ((s16) temp3 < 0) {
            out3 = 0;
        } else {
            out3 = temp3;
        }
        arg0->field_16 = out3;
    }
    if (arg0->field_34 & 0x20) {
        value = (s16) arg0->field_16;
        amount = 1;
        if (value >= 0x64) {
            amount = 3;
            if (value < 0xC8) {
                amount = 2;
            }
        }
        temp4 = arg0->field_AC + amount;
        arg0->field_AC = temp4;
        if ((s16) temp4 >= 0x100) {
            out4 = 0xFF;
        } else {
            out4 = temp4;
        }
        arg0->field_AC = out4;
    }
    if (arg0->field_30 == 1) {
        if (((struct Ovl11B264StateView *) D_8006C838)->field_E4C8 != 0) {
            temp5 = arg0->field_16 - 5;
            arg0->field_16 = temp5;
            if ((s16) temp5 < 0) {
                out5 = 0;
            } else {
                out5 = temp5;
            }
            arg0->field_16 = out5;
        }
    }
    if ((arg0->field_1A != ((struct Ovl11B264StateView *) D_8006C838)->field_44B8) &&
        (arg0->field_1C == ((struct Ovl11B264StateView *) D_8006C838)->field_44BA)) {
        if (arg0->field_1E <= ((struct Ovl11B264StateView *) D_8006C838)->field_44BC) {
            arg0->field_0 = 0x15F;
            arg0->field_34 |= 0x8000;
            ovl_11_func_8010B830(arg0);
        }
    }
    arg0->field_34 &= ~0x26;
    Ovl11ResetTimedSpriteEffect(arg0->field_A8);
    return 0;
}
