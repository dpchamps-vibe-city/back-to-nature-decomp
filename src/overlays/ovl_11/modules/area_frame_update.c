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
#include "ovl11_primary_object_update.h"
#include "ovl11_primary_object_draw.h"
#include "ovl11_moving_items.h"
#include "ovl11_condition_camera_jitter.h"
#include "ovl11_interaction_dispatch.h"
#include "ovl11_scene_record_render.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_primary_object_motion.h"
#include "ovl11_pending_event_queue.h"
#include "ovl11_area_frame_update_dispatch.h"
#include "ovl11_menu_frame_update.h"
#include "ovl11_clock_scene_completion.h"
#include "ovl11_scene_setup_install.h"
#include "psyq/libapi.h"
#include "ovl11_object_archive_pipeline.h"
#include "ovl11_interaction_volume_activation.h"
#include "ovl11_object_record_state.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_scene_record_frame.h"

void Ovl11UpdateClockHudVisibility(void);
void Ovl11UpdateAreaFrame(void);

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
        Ovl11ReloadObjectSoundBank();
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
            Ovl11DrawSceneRecords();
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

void Ovl11UpdateAreaFrame(void) {
    gOvl11AreaFrameResult = 2;
    Ovl11RunClockAdvance();
    Ovl11UpdateClockAndEvents();
    Ovl11TriggerTimedObjectEvents();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11ResetCollisionQueryCount();
    Ovl11UpdateWeatherEffect();
    Ovl11UpdateMenuFrame();
    Ovl11UpdateAreaGridTrigger();
    Ovl11UpdateSceneCameraOffset();
    Ovl11UpdatePrimaryObject();
    Ovl11UpdateChoiceMessages();
    Ovl11UpdateSelectionMessage();
    if (((struct Ovl11FrameWork *) &D_8007AFF0)->mode == 0) {
        gOvl11AreaFrameResult = Ovl11ActivateInteractionVolume((Ovl11C7010Object *) &gPrimaryObjectBase);
    }
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    if (gOvl11AreaFrameResult == 2) {
        Ovl11UpdatePrimaryObjectMotion();
    } else {
        ((struct Ovl11FrameState *) D_8006C838)->field_52D8 = 0;
        ((struct Ovl11FrameState *) D_8006C838)->field_52E0 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253A8 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253AA = 0;
    }
    Ovl11UpdateVramAnimations();
    Ovl11UpdateSpriteRotations();
    Ovl11JitterCameraTargetZ(1);
    Ovl11StepMapCameraVerticalOffset();
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11ApplySceneCamera();
    ApplyGteSceneState();
    SCRATCH_STACK_END();
    if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
        Ovl11DrawSceneRecords();
        Ovl11DrawSceneGeometry();
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        Ovl11DrawPrimaryObjectFrame((Ovl11SpritePositionView *) ((struct Ovl11FrameState *) D_8006C838)->position, 1);
        Ovl11UpdateMovingItemRecords();
        Ovl11UpdateSceneEffects();
        SCRATCH_STACK_END();
    }
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11EvaluatePendingConditions();
    Ovl11ServiceActiveRecordSlot();
    Ovl11UpdateObjectEvents();
    Ovl11UpdateObjectRecordPool();
    Ovl11RunAreaFrameUpdates();
    Ovl11UpdateInteractionDispatch();
    Ovl11ServiceSceneRecordFrame();
    Ovl11UpdateSpriteEffects();
    Ovl11UpdateEventQueueGate();
    UpdateMessageWindow();
    Ovl11ConsumeDeferredClockAdvance();
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
