/*
 * Area frame-phase dispatch and its ordered update/HUD-policy steps.
 * Public callback: ovl11_area_frame_update.h. The two service steps retain
 * external linkage, but have no callers outside this module.
 * Scratchpad stack brackets are engine calls' execution boundaries, not
 * interchangeable scopes: keep their order and their matching begin/end.
 * Original ovl_11:800C14A0-800C19B0. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_area_frame_update.h"
#include "ovl11_collision_query_budget.h"
#include "gte_scene_state.h"
#include "cd_audio.h"
#include "psyq/stddef.h"
#include "game_types.h"
#include "scratchpad.h"
#include "psyq/libgte.h"
#include "sprite_rotation.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl11_scene_camera_lighting.h"
#include "ovl11_area_mode_sequence.h"
#include "ovl11_selection_message.h"
#include "ovl11_clock_calendar.h"
#include "message_window.h"
#include "ovl11_choice_messages.h"
#include "ovl11_sprite_effect_pool.h"
#include "psyq/abs.h"
#include "ovl11_object_event_script.h"
#include "psyq/memory.h"
#include "ovl11_vram_animation_slots.h"
#include "map_texture_camera.h"
#include "area_grid_triggers.h"
#include "ovl11_weather_effect.h"
#include "ovl11_object_events.h"
#include "ovl11_clock_hud.h"
#include "game_flags.h"

void Ovl11UpdateClockHudVisibility(void);
void Ovl11UpdateAreaFrame(void);
void ovl_11_func_800E3AA4(void);
void ovl_11_func_800DCC5C(void);

void Ovl11RunAreaFramePhase(void) {
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x18000) {
        gRandomState = 1;
    }
    switch (((struct Ovl11FrameWork *) &D_8007AFF0)->dispatch) {
    case 0:
        Ovl11UpdateClockHudVisibility();
        if ((((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 5) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x18) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x19)) {
            Ovl11UpdateAreaFrame();
        }
        Ovl11UpdateAreaMode();
        ovl_11_func_800E3AA4();
        PollCdAudio();
        break;
    case 1:
    case 2:
        break;
    case 3:
        Ovl11UpdateMapCameraMode();
        Ovl11UpdateSceneCameraOffset();
        Ovl11ApplySceneCamera();
        ApplyGteSceneState();
        if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
            ovl_11_func_800DCC5C();
            Ovl11DrawSceneGeometry();
        }
        break;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x40000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags &= 0xFF7BFFFF;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x20000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags = (((struct Ovl11FrameState *) D_8006C838)->flags & 0xFFFDFFFF) | 0x800000;
    }
}

void ovl_11_func_800F65B8 (void);
void ovl_11_func_800C401C (void);
s32 ovl_11_func_800CD9A0 (Ovl11C7010Object *arg0);
void ovl_11_func_800CAADC (void);
void ovl_11_func_800C056C(s32 unused);
void ovl_11_func_800DCC5C (void);
void ovl_11_func_800CA24C(Ovl11SpritePositionView *position, s32 unused);
void ovl_11_func_800CCB90 (void);
void ovl_11_func_800DC148 (void);
s32 ovl_11_func_800F0F50 (void);
void ovl_11_func_800DBAB0 (void);
void ovl_11_func_8010C7F0 (void);
void ovl_11_func_800CE834 (void);
s32 ovl_11_func_801189C8 (void);
void ovl_11_func_800F3FB4 (void);
void ovl_11_func_800F2400 (void);
void ovl_11_func_800DE46C (void);

void Ovl11UpdateAreaFrame(void) {
    gOvl11AreaFrameResult = 2;
    Ovl11RunClockAdvance();
    Ovl11UpdateClockAndEvents();
    Ovl11TriggerTimedObjectEvents();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11ResetCollisionQueryCount();
    Ovl11UpdateWeatherEffect();
    ovl_11_func_800F65B8();
    Ovl11UpdateAreaGridTrigger();
    Ovl11UpdateSceneCameraOffset();
    ovl_11_func_800C401C();
    Ovl11UpdateChoiceMessages();
    Ovl11UpdateSelectionMessage();
    if (((struct Ovl11FrameWork *) &D_8007AFF0)->mode == 0) {
        gOvl11AreaFrameResult = ovl_11_func_800CD9A0((Ovl11C7010Object *) &D_80071A00);
    }
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    if (gOvl11AreaFrameResult == 2) {
        ovl_11_func_800CAADC();
    } else {
        ((struct Ovl11FrameState *) D_8006C838)->field_52D8 = 0;
        ((struct Ovl11FrameState *) D_8006C838)->field_52E0 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253A8 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253AA = 0;
    }
    Ovl11UpdateVramAnimations();
    Ovl11UpdateSpriteRotations();
    ovl_11_func_800C056C(1);
    Ovl11StepMapCameraVerticalOffset();
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11ApplySceneCamera();
    ApplyGteSceneState();
    SCRATCH_STACK_END();
    if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
        ovl_11_func_800DCC5C();
        Ovl11DrawSceneGeometry();
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        ovl_11_func_800CA24C((Ovl11SpritePositionView *) ((struct Ovl11FrameState *) D_8006C838)->position, 1);
        ovl_11_func_800CCB90();
        ovl_11_func_800DC148();
        SCRATCH_STACK_END();
    }
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    ovl_11_func_800F0F50();
    ovl_11_func_800DBAB0();
    Ovl11UpdateObjectEvents();
    ovl_11_func_8010C7F0();
    ovl_11_func_800CE834();
    ovl_11_func_801189C8();
    ovl_11_func_800F3FB4();
    Ovl11UpdateSpriteEffects();
    ovl_11_func_800F2400();
    UpdateMessageWindow();
    ovl_11_func_800DE46C();
    SCRATCH_STACK_END();
}

/* Clock advancement hides the HUD. The 0x10000000 latch instead preserves
 * its current visibility; the remaining branches apply explicit policy. */
void Ovl11UpdateClockHudVisibility(void) {
    char *farWork;
    s32 gameFlag2;

    if (Ovl11IsClockAdvanceActive() == 1) {
        Ovl11DisableClockHud();
        return;
    }
    if (((Ovl11ClockHudPolicyState *)D_8006C838)->flags & 0x10000000) {
        return;
    }
    gameFlag2 = GetGameFlag(2);
    if (gameFlag2 == 1) {
        farWork = (char *)&D_8007AFF0;
        if (*(u16 *)(*(s32 *)(farWork + 0x25388) + 8) & 0x1000) {
            if (GetGameFlag(0xB0) == 1) {
                Ovl11DisableClockHud();
                return;
            }
            if (*(u16 *)(*(s32 *)(farWork + 0x25388) + 8) & 0x1000) {
                goto selectionPolicy;
            }
        }
        if (GetGameFlag(5) == 1) {
            Ovl11DisableClockHud();
            return;
        }
        if (GetGameFlag(4) == 1) {
            Ovl11EnableClockHud();
            return;
        }
    }
selectionPolicy:
    if (((Ovl11ClockHudPolicyState *)D_8006C838)->selectionValue != 0) {
        Ovl11EnableClockHud();
        return;
    }
    Ovl11DisableClockHud();
}
