/*
 * Object archive context transitions. The loader enters before loading and
 * on release with queued records; otherwise it leaves. These are paired
 * flag/control transitions, not a saved-state push/pop or inverse resets.
 * Public contract: ovl11_object_load_context.h.
 */

#include "common.h"
#include "game_types.h"
#include "game_flags.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_camera_position_control.h"
#include "ovl11_object_load_context.h"
#include "ovl11_camera_record_selection.h"
#include "ovl11_flag_group_command.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_stage_mode1_record.h"

void Ovl11EnterObjectLoadContext(void) {
    struct Ovl11ObjectLoadEntryView *state;

    SetGameFlag(3U, 1U);
    if (!(((struct Ovl11ObjectLoadEntryView *) D_8006C838)->controlFlags & 0x20000010)) {
        Ovl11SetPositionControlFlags(0, 0, 0, 0);
    }
    if (!(((u16 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header)[4] & 0x1000)) {
        if ((u32) (((struct Ovl11ObjectLoadEntryView *) D_8006C838)->currentObject[1] - 0x15F) >= 0x32U) {
            Ovl11SetFlagGroupMode(0, 0, 0, 0);
        }
    }
    Ovl11ResetTimedSpriteEffect(gObjectWorkTimedSpriteEffect);
    /* Keep the shared effect-pair base: it also addresses the work state. */
    state = (struct Ovl11ObjectLoadEntryView *) ((u8 *) gObjectWorkTimedSpriteEffect - 0x5494);
    state->spriteSourceState = 1;
    state->workFlags &= ~0x2000;
}

void Ovl11LeaveObjectLoadContext(void) {
    SetGameFlag(3, 0);
    Ovl11SelectCameraPositionRecord(-2, 0, 0, 0);
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
        Ovl11SetStageMode1PositionPreset(0);
    }
}
