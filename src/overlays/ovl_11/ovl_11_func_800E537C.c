#include "common.h"
#include "game_flags.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_item_slot_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/abs.h"
#include "ovl11_camera_position_control.h"

s32 ovl_11_func_800E880C(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
/* The original calls explicitly supply zero in both trailing argument slots. */
void ovl_11_func_8010941C(s32 arg0);

void ovl_11_func_800E537C(void) {
    SetGameFlag(3, 0);
    ovl_11_func_800E880C(-2, 0, 0, 0);
    Ovl11SetPositionControlFlags(1, 0, 0, 0);
    ((struct Ovl11ResetState *) &D_8006C838)->field_51EA = -1;
    ((struct Ovl11ResetState *) &D_8006C838)->field_51F0 = 0;
    ((struct Ovl11ResetState *) &D_8006C838)->field_5234 &= ~0x100000;
    ((struct Ovl11ResetState *) &D_8006C838)->field_4450 &= ~2;
    ((struct Ovl11ResetState *) &D_8006C838)->field_51FE &= ~8;
    if ((Ovl11HasEmptyItemSlot(1, 0, 0, 0) == 0) &&
        (((struct Ovl11ResetState *) &D_8006C838)->field_5204 != 0xC)) {
        ((struct Ovl11ResetState *) &D_8006C838)->field_5234 =
            (((struct Ovl11ResetState *) &D_8006C838)->field_5234 & ~0x4000) | 0x8000;
    }
    if (*((struct Ovl11ResetState *) &D_8006C838)->field_DDD4 == 0x36) {
        Ovl11PublishCurrentPoolRecord();
    }
    if (((struct Ovl11ResetState *) &D_8006C838)->field_91DC == 0x36) {
        ovl_11_func_8010941C(0);
    }
}
