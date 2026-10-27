/*
 * Clock-queue event callbacks 0..4 (ovl_11:800DDDB4-800DE3A0).
 * Their return values implement the queue's retry/completion protocol; the
 * day-scene rebuild deliberately yields between its seven phases. Resource
 * installation order and the blocking sound wait are significant.
 * Public contracts: ovl11_clock_event_callbacks.h. Membership and conservative
 * boundaries: notes/file-groupings.md. Shared resident storage stays external.
 */

#include "common.h"
#include "ovl11_clock_event_callbacks.h"
#include "game_types.h"
#include "screen_fade.h"
#include "sprite_rotation.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vertex_blend.h"
#include "ovl11_daily_conditions.h"
#include "game_flags.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_vram_animation_slots.h"
#include "ovl11_scene_record_pool.h"
#include "ovl11_area_mode_sequence.h"
#include "ovl11_weather_effect.h"
#include "psyq/abs.h"
#include "ovl11_object_events.h"
#include "ovl11_grid_cell_queries.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_moving_items.h"
#include "psyq/libetc.h"
#include "ovl11_progress_status.h"
#include "ovl11_record_flag_counters.h"
#include "ovl11_grid_transition_passes.h"
#include "ovl11_availability_event_reset.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_scene_setup_install.h"
#include "ovl11_default_position.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_pending_event_queue.h"
#include "ovl11_hour_counter_adjustment.h"
#include "ovl11_record_pool_lifecycle.h"
#include "ovl11_event_schedule.h"
#include "ovl11_queued_item_interaction.h"
#include "ovl11_grid_growth.h"
#include "ovl11_clock_scene_completion.h"
#include "ovl11_calendar_record_gate.h"
#include "ovl11_scene_header_capture.h"

s32 Ovl11ClockAcknowledgeEvent(void) {
    return 1;
}

s32 Ovl11ClockShiftValueHistory(void) {
    s32 *residentWords;
    s32 *newest;
    s32 *pair;
    s32 historyIndex;

    /* Copy from oldest to newest so overlapping history slots survive. */
    historyIndex = 2;
    do {
        residentWords = (s32 *)&D_8006C838;
        pair = residentWords + (historyIndex + 1) * 2;
        pair[0x1271] = pair[0x126F];
        pair[0x1272] = pair[0x1270];
        historyIndex--;
    } while (historyIndex >= 0);
    newest = (s32 *)&D_8006C838;
    newest[0x1271] = 0;
    newest[0x1272] = 0;
    return 1;
}

void ovl_11_func_800CCCC0 (void);
void ovl_11_func_800EEECC (void);
void ovl_11_func_8010C9B4 (void);
void ovl_11_func_800F40A4 (void);
void ovl_11_func_800F3D40 (void);

s32 Ovl11ClockRebuildDayScene(void) {
    s32 result;
    s32 id;
    s32 offset;
    s32 sameArea;
    Ovl11DDDFCState *phase0;

    SetGameFlag(3, 1);
    result = 0;
    ((Ovl11DDDFCState *)D_8006C838)->flags |= 0x08000000;
    ((s32 *)gPad0Input)[2] = 0;
    ((s32 *)gPad0Input)[1] = 0;
    ((s32 *)gPad0Input)[0] = 0;
    switch (((Ovl11DDDFCState *)D_8006C838)->phase) {
    case 0:
        phase0 = (Ovl11DDDFCState *)D_8006C838;
        if (phase0->control & 4) {
            phase0->phase += 2;
            break;
        }
        if (GetGameFlag(0x47) == 0) {
            if (IsScreenFadeComplete() != 1) {
                break;
            }
            if (GetScreenFadeRampMode() != 2) {
                BeginScreenFadeIncrease(0x2D, 0, 2);
            }
        }
        phase0->phase++;
        break;
    case 1:
        if (IsScreenFadeComplete() == 1) {
            ((Ovl11DDDFCState *)D_8006C838)->phase++;
            ((Ovl11DDDFCState *)D_8006C838)->flags |= 0x800000;
        }
        break;
    case 2:
        SetGameFlag(0x73, ((Ovl11DDDFCState *)D_8006C838)->mode == 3);
        Ovl11AdvanceSeasonalGridGrowth();
        Ovl11RunRecordPoolSecondPass();
        Ovl11ApplyCurrentDayCondition();
        Ovl11ChooseNextDayCondition();
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        break;
    case 3:
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        break;
    case 4:
        if (IsScreenFadeComplete() == 1) {
            ovl_11_func_800CCCC0();
            Ovl11ResetSpriteRotations();
            Ovl11UpdateProgressTimers();
            ((Ovl11SetupWorkView *)&D_8007AFF0)->alternateId = -1;
            offset = ((Ovl11DDDFCState *)D_8006C838)->variant * 4 + 0x50;
            id = ((Ovl11DDDFCState *)D_8006C838)->index + offset;
            sameArea = ((Ovl11SetupWorkView *)&D_8007AFF0)->id == id;
            ((Ovl11SetupWorkView *)&D_8007AFF0)->id = id;
            Ovl11InstallSceneSetup((u16)((Ovl11SetupWorkView *)&D_8007AFF0)->id, 0x8000, sameArea);
            Ovl11UpdateEventSlotCounters();
            Ovl11UpdateRecordFlagCounters();
            Ovl11LoadIndexedSpriteRecords(*(s16 *)(((Ovl11SetupWorkView *)&D_8007AFF0)->header + 0xA));
            Ovl11LoadCalendarSpriteSet(*(s16 *)(((Ovl11SetupWorkView *)&D_8007AFF0)->header + 0xC), 0);
            Ovl11LoadIndexedSpriteSet(0);
            Ovl11InitVramAnimationSlots();
            Ovl11ResetVertexBlendPool();
            Ovl11ClearSceneRecords();
            Ovl11InstallSceneRecords();
            Ovl11LoadIndexedSpriteRecords(((Ovl11SetupWorkView *)&D_8007AFF0)->music);
            Ovl11RequestVramAnimations((Ovl11SetupHeader *)gLoadedAreaHeaderCopy);
            ((Ovl11SetupWorkView *)&D_8007AFF0)->current = ((Ovl11SetupWorkView *)&D_8007AFF0)->previous;
            ((Ovl11SetupWorkView *)&D_8007AFF0)->header = gLoadedAreaHeaderCopy;
            Ovl11RegisterPoolSpriteRotations((s32)gLoadedAreaHeaderCopy);
            Ovl11ReadDefaultPosition(((Ovl11DDDFCState *)D_8006C838)->position);
            Ovl11ResetMovingItemRecords();
            ((Ovl11DDDFCState *)D_8006C838)->status = (((Ovl11DDDFCState *)D_8006C838)->status | 0x4000) & 0xFFF07FFF;
            Ovl11CaptureSceneHeaderKey();
            ovl_11_func_800EEECC();
            Ovl11StageFixedSpriteBank();
            Ovl11InitEventSchedules(gGraphicsHeapBase + 0x4290);
            Ovl11RebuildObjectEventsForTime();
            Ovl11ClearEventSlotTimeOrigins();
            Ovl11ResetWeatherParticles();
            Ovl11ResetAmbientTintFromTable();
            Ovl11SetWeatherParticleMode(((Ovl11DDDFCState *)D_8006C838)->mode);
            ((Ovl11SetupWorkView *)&D_8007AFF0)->state = 0;
            ((Ovl11DDDFCState *)D_8006C838)->phase++;
            if (GetGameFlag(0x48) == 0) {
                Ovl11RefreshAreaSoundBank();
                while (IsSoundTransferComplete() == 0) {
                    UpdateMasterVolumeAndSoundTransfer();
                }
                OpenSoundSequencesAndStart();
                Ovl11ApplyAreaSoundVolume();
            }
        }
        break;
    case 5:
        Ovl11UpdateQueuedItemInteraction();
        Ovl11AdvanceTrackedRecordCounters();
        ovl_11_func_8010C9B4();
        ovl_11_func_800F40A4();
        ovl_11_func_800F3D40();
        Ovl11ServiceEventQueues();
        Ovl11UpdateCalendarRecordGate();
        Ovl11ProbePoolRecordTransitions();
        ((Ovl11DDDFCState *)D_8006C838)->status &= 0xFBFFFFFF;
        switch (((Ovl11DDDFCState *)D_8006C838)->mode) {
        case 1:
            Ovl11ProcessMainGridCells();
            Ovl11PassGridKind3FTransitions(0);
            break;
        case 2:
            Ovl11PassGridKind3FTransitions(1);
            break;
        case 3:
        case 4:
            Ovl11PassGridEntryResets();
            Ovl11TryClearAvailabilityEvent();
            break;
        }
        ((Ovl11DDDFCState *)D_8006C838)->phase++;
        if (Ovl11IsClockAdvanceActive() == 0) {
            Ovl11SetClockAdvanceRefresh(0);
            Ovl11CompleteClockSceneEvent();
        } else {
            Ovl11SetClockAdvanceRefresh(1);
        }
        break;
    case 6:
        SetGameFlag(3, 0);
        ((Ovl11DDDFCState *)D_8006C838)->flags &= 0xF7FFFFFF;
        result = 1;
        break;
    }
    return result;
}

void ovl_11_func_800F3FF4(s16 arg0);

s32 Ovl11ClockRefreshAreaSprites(void) {
    Ovl11LoadOffsetSpriteSet(((struct struct_8006C838_button *)D_8006C838)->field_44BA);
    ovl_11_func_800F3FF4(((struct struct_8006C838_button *)D_8006C838)->field_44BA);
    Ovl11ClearQueuedItemInteraction();
    return 1;
}

void ovl_11_func_8010C95C(void);

s32 Ovl11ClockApplyHourUpdates(void) {
    Ovl11RunRecordPoolFirstPass();
    ovl_11_func_8010C95C();
    Ovl11ApplyHourCounterDeltas();
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0xC) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0x12) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    return 1;
}
