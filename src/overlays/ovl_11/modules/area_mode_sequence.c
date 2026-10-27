/*
 * Area-mode transitions and entry sequencing for ovl_11. The mode machine
 * commits a selected record, starts/polls its archive read, waits for fades,
 * and requests resident scene changes. The separate entry-step machine
 * rebuilds scene services and coordinates sound-bank transfer and playback.
 *
 * Private read/sound state joins the eight original entries BBC74..BCD28;
 * two contiguous jump tables are pooled in their original order. Globals
 * remain externally stored overlay data, not new tentative definitions.
 * See notes/file-groupings.md for the complete membership and boundaries.
 */

#include "common.h"
#include "ovl11_area_mode_sequence.h"
#include "scratchpad.h"
#include "game_types.h"
#include "screen_fade.h"
#include "scene_selection.h"
#include "gte_scene_state.h"
#include "aux_sprite_display.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "sound_driver.h"
#include "random.h"
#include "render_packet_pool.h"
#include "ovl11_sprite_effect_pool.h"
#include "sprite_rotation.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl11_scene_camera_lighting.h"
#include "ovl11_vertex_blend.h"
#include "ovl11_choice_messages.h"
#include "game_flags.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vram_animation_slots.h"
#include "ovl11_scene_record_pool.h"
#include "mode6_texture_refresh.h"
#include "map_cell_texture.h"
#include "area_grid_triggers.h"
#include "ovl11_weather_effect.h"
#include "ovl11_primary_object_requests.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "psyq/abs.h"
#include "ovl11_progress_status.h"
#include "ovl11_mode6_grid.h"
#include "ovl11_area_selection_setup.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_default_position.h"
#include "ovl11_item_spawn_placement.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_area_scene_load.h"
#include "ovl11_clock_event_callbacks.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_primary_object_sprite_slots.h"
#include "ovl11_held_item_replacement.h"
#include "ovl11_primary_object_input_override.h"
#include "ovl11_scene_header_capture.h"
#include "ovl11_state_object_pool_clear.h"

void Ovl11StepAreaModeState(void);

/* The existing scratch-stack ABI must enclose the state-machine call. */
void Ovl11UpdateAreaMode(void) {
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11StepAreaModeState();
    SCRATCH_STACK_END();
}

/* Overlay names for the matched setters at 80011F44 and 80011F50. */
void SetVal8005E278(s32 arg0);
void SetVal8005E27C(s32 arg0);
void ovl_11_func_800BCF28(void);
void ovl_11_func_800CD624(void);

#define WORK ((Ovl11BBCB8Work *)&D_8007AFF0)
#define STATUS ((Ovl11BBCB8Status *)D_8006C838)

void Ovl11StepAreaModeState(void) {
    u16 selection;

    switch (WORK->state) {
    case 26:
        WORK->state = 27;
        BeginScreenFadeIncrease(10, 0, 2);
        BeginMasterVolumeFadeOut(10);
        break;
    case 27:
        if (IsScreenFadeComplete() == 1) {
            RequestSceneChange(13);
        }
        break;
    case 0:
        gOvl11AreaReadStarted = 0;
        break;
    case 1:
        if (!(STATUS->objectFlags & 8)) {
            WORK->state = 2;
            if (gOvl11SelectedInteractionVolume->selectionId != 0xFFFE) {
                Ovl11BeginPrimaryObjectInputOverride(20);
            } else {
                Ovl11BeginPrimaryObjectInputOverride(27);
            }
        } else if (STATUS->objectFlags & 0x800000) {
            WORK->state = 3;
            STATUS->objectFlags |= 0x400;
            STATUS->flags |= 0x8000000;
        }
        break;
    case 2:
        if (!(STATUS->objectFlags & 8)) {
            if (gOvl11SelectedInteractionVolume != 0) {
                if (gOvl11SelectedInteractionVolume->selectionId == 0xFFFE) {
                    WORK->state = 0;
                    if (gOvl11SelectedInteractionVolume->actionCode == 0x2E) {
                        STATUS->selectionFlags |= 0x80;
                    }
                    if (gOvl11SelectedInteractionVolume->actionCode == 0x20 && gOvl11SelectedInteractionVolume->direction == 2) {
                        STATUS->selectionFlags |= 0x20;
                    }
                } else {
                    WORK->state = 3;
                    STATUS->objectFlags |= 0x400;
                    STATUS->flags |= 0x8000000;
                }
            } else {
                WORK->state = 0;
            }
        }
        break;
    case 3:
        Ovl11ResetPrimaryObjectRequest();
        BeginScreenFadeFeedback(20);
        Ovl11ProbeActivePoolGate();
        gOvl11AreaSoundEffectPending = 0;
        if (gOvl11SelectedInteractionVolume->actionCode != 99) {
            if (!(gOvl11SelectedInteractionVolume->flags & 0x200)) {
                gOvl11AreaSoundEffectPending = 1;
            }
        }
        selection = gOvl11SelectedInteractionVolume->selectionId;
        WORK->object = gOvl11SelectedInteractionVolume;
        WORK->state = 5;
        STATUS->objectFlags &= ~0x400;
        WORK->savedSelection = selection;
        gOvl11PendingAreaReadIndex = Ovl11ResolveAreaSelection(selection);
        Ovl11ReadAreaResource(gOvl11PendingAreaReadIndex, 1);
        gOvl11AreaReadStarted = 1;
        if (STATUS->mode == 2) {
            Ovl11ServiceActiveRecordSlot();
        }
        break;
    case 24:
        Ovl11ResetPrimaryObjectRequest();
        WORK->object = 0;
        WORK->state = 25;
        STATUS->flags |= 0x8000000;
        gOvl11PendingAreaReadIndex = Ovl11ResolveAreaSelection(WORK->savedSelection);
        Ovl11ReadAreaResource(gOvl11PendingAreaReadIndex, 1);
        gOvl11AreaReadStarted = 1;
        break;
    case 13:
    case 30:
        if (IsScreenFadeComplete() == 1) {
            WORK->state = 0;
        }
        break;
    case 5:
    case 25:
        STATUS->flags |= 0x800000;
        if (gOvl11AreaReadStarted != 0 && Ovl11ReadAreaResource(gOvl11PendingAreaReadIndex, 0) == 0) {
            break;
        }
    case 4:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 28:
        STATUS->flags |= 0x800000;
        STATUS->flags &= ~0x10000000;
    case 12:
    case 31:
        if (IsScreenFadeComplete() == 1) {
            if (STATUS->mode == 2) {
                Ovl11ServiceActiveRecordSlot();
            }
            SetVal8005E278(1);
            SetVal8005E27C(1);
            STATUS->flags &= ~0x800000;
            switch (WORK->state) {
            case 28:
                RequestSceneChange(16);
                break;
            case 14:
                RequestSceneChange(7);
                break;
            case 15:
                RequestSceneChange(8);
                break;
            case 16:
                RequestSceneChange(9);
                break;
            case 17:
                RequestSceneChange(10);
                break;
            case 18:
                RequestSceneChange(3);
                StopSoundSequence(0);
                break;
            case 19:
                RequestSceneChange(4);
                break;
            case 20:
                RequestSceneChange(5);
                break;
            case 21:
                RequestSceneChange(11);
                break;
            case 22:
                RequestSceneChange(12);
                break;
            case 12:
            case 31:
                STATUS->objectFlags |= 0x400000;
                ovl_11_func_800BCF28();
                if (WORK->state == 31) {
                    WORK->state = 30;
                    SetGameFlag(0x9D, 1);
                    SetGameFlag(0x47, 1);
                } else {
                    WORK->state = 13;
                }
                ovl_11_func_800CD624();
                break;
            case 5:
                if (gOvl11AreaSoundEffectPending != 0) {
                    PlaySoundEffect(0x4C);
                }
            case 25:
                gOvl11AreaEntryStep = 3;
                STATUS->objectFlags &= ~0x402;
                STATUS->counter--;
                WORK->selection = WORK->savedSelection;
                break;
            case 4:
                gOvl11AreaEntryStep = 3;
                STATUS->counter--;
                WORK->selection++;
                if (WORK->selection == 256) {
                    WORK->selection = 0;
                }
                if (GetGameFlag(0x9C) == 1) {
                    WORK->selection = D_8005E32C;
                    SetGameFlag(0x9C, 0);
                }
                STATUS->direction = WORK->selection - WORK->selection / 4 * 4;
                Ovl11ClockRefreshAreaSprites();
                WORK->object = 0;
                break;
            }
        }
        break;
    case 23:
        if (IsScreenFadeComplete() == 1) {
            WORK->pendingMode = 3;
            WORK->state = 0;
        }
        break;
    case 6:
        if (IsScreenFadeComplete() == 1) {
            WORK->state = 0;
            RequestSceneChange(17);
        }
        break;
    }
}

#undef WORK
#undef STATUS

void Ovl11ResetAreaModeStep(void) {
    char *work = (char *)&D_8007AFF0;
    s32 *counter;

    gOvl11AreaEntryStep = 0;
    *(s32 *)(work + 0x2548C) = 0;
    counter = (s32 *)&D_8006C838;
    counter[0x1122] += 1;
}

void Ovl11ResetAreaScene(void);
void Ovl11RefreshAreaSoundBank(void);
/* Resident sprite-display disable at exe:8001B100. */
void Ovl11ApplyAreaSoundVolume(void);
void ovl_11_func_800F4114(void);
void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
/* The callee stores argument 1 as a halfword; no caller-side narrowing. */
void ovl_11_func_800F1038(u16 arg0, s32 arg1);

/* Cases 0/9 and 2/10 deliberately share tails. Calls may wait for sound
 * transfers or repeatedly drive the state updater; this is not reentrant. */
void Ovl11RunAreaEntrySteps(void) {
    s32 value;
    Ovl11AreaInitView *state;

    switch (gOvl11AreaEntryStep) {
    case 0:
        Ovl11PrepareAreaScene();
    case 9:
        ((Ovl11MovementState *)D_8006C838)->flagsC |= 0x8000000;
        ((Ovl11MovementState *)D_8006C838)->flags10 |= 0x8000000;
        Ovl11AreaResourceEntryHook();
        SetGameFlag(3, 1);
        gOvl11AreaEntryStep++;
        break;
    case 1:
        Ovl11InitAreaSpriteSource();
        gOvl11AreaEntryStep++;
        break;
    case 2:
        state = (Ovl11AreaInitView *)D_8006C838;
        if (state->flags & 0x2000) {
            ((Ovl11FarInitView *)&D_8007AFF0)->field_2547C = 0;
        } else {
            s32 offset;

            ((Ovl11FarInitView *)&D_8007AFF0)->field_2547C =
                state->field_44BA + (offset = state->field_44CE * 4 + 0x50);
        }
    case 10:
        if (IsSoundTransferComplete()) {
            gOvl11AreaEntryStep++;
            InitRenderPacketPools();
            InitGteSceneState();
        }
        break;
    case 3:
    case 11:
        gOvl11AreaEntryStep++;
        break;
    case 4:
    case 12:
        Ovl11ResetAreaScene();
        gOvl11AreaEntryStep++;
        break;
    case 5:
    case 13:
        gOvl11AreaSoundRestartNeeded = 0;
        Ovl11RefreshAreaSoundBank();
        gOvl11AreaEntryStep++;
        break;
    case 6:
    case 14:
        if (IsSoundTransferComplete()) {
            gOvl11AreaEntryStep++;
        }
        break;
    case 7:
    case 15:
        DisableAuxSpriteDisplay();
        if (gOvl11AreaSoundRestartNeeded && !(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 0x4000)) {
            OpenSoundSequencesAndStart();
            BeginMasterVolumeFadeIn(10);
        }
        ((Ovl11AreaInitView *)D_8006C838)->field_4450 &= ~0x4000;
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0x19) {
            BeginScreenFadeDecrease(10, 0, 2);
        }
        ((Ovl11AreaInitView *)D_8006C838)->field_4488++;
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 == 0xC) {
            Ovl11ReadDefaultPosition((s32 *)((struct Ovl11FrameState *)D_8006C838)->position);
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[0] = ((struct Ovl11FrameState *)D_8006C838)->position[0];
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[1] = ((struct Ovl11FrameState *)D_8006C838)->position[2];
            ((Ovl11MovementCameraWork *)&D_8007AFF0)->coordinates[2] = ((struct Ovl11FrameState *)D_8006C838)->position[4];
        }
        if (!(((Ovl11MovementState *)D_8006C838)->flagsC & 0x2000)) {
            if (!GetGameFlag(2)) {
                SetGameFlag(3, 0);
            }
        } else {
            SetGameFlag(3, 1);
        }
        Ovl11CaptureSceneHeaderKey();
        Ovl11ApplyAreaSoundVolume();
        Ovl11ClearEligibleStateObjects(0);
        Ovl11SpawnTimedItemPlacements();
        Ovl11ClearEventSlotScheduleChanges();
        ovl_11_func_800F4114();
        Ovl11ClearSpriteEffects();
        if (((Ovl11AreaInitView *)D_8006C838)->field_4450 & 0x40) {
            ((Ovl11AreaInitView *)D_8006C838)->field_4450 &= ~0x40;
            if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == 2) {
                ovl_11_func_800F0FB0(8);
                ovl_11_func_800F0F50();
                Ovl11ServiceActiveRecordSlot();
                if (GetGameFlag(2) == 1) {
                    for (;;) {
                        if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                            Ovl11ServiceActiveRecordSlot();
                            break;
                        }
                        Ovl11ServiceActiveRecordSlot();
                        if (GetGameFlag(2) == 0) {
                            break;
                        }
                    }
                }
            }
            if (!(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 4) && !GetGameFlag(2)) {
                Ovl11ReplaceHeldItem((Ovl11C7010Object *)((Ovl11AreaInitView *)D_8006C838)->field_51C8, Rand(4) + 0x11B, 0);
            }
        } else {
            if (((Ovl11MovementState *)D_8006C838)->flagsC & 0x2000) {
                ovl_11_func_800F1038(0x74, 1);
                value = ((Ovl11MovementState *)D_8006C838)->flagsC & ~0x2000;
                ((Ovl11MovementState *)D_8006C838)->flagsC = value;
            } else {
                ovl_11_func_800F0FB0(2);
            }
            ovl_11_func_800F0F50();
            Ovl11ServiceActiveRecordSlot();
            if (GetGameFlag(2) == 1) {
                for (;;) {
                    if (((struct Ovl11E4280State *)D_8006C838)->selector == 1) {
                        Ovl11ServiceActiveRecordSlot();
                        break;
                    }
                    Ovl11ServiceActiveRecordSlot();
                    if (GetGameFlag(2) == 0) {
                        break;
                    }
                }
            }
        }
        if (((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 != 0x18) {
            ((Ovl11FarInitView *)&D_8007AFF0)->field_254A0 = 0;
            value = ((Ovl11MovementState *)D_8006C838)->flagsC & ~0x8000000;
            ((Ovl11MovementState *)D_8006C838)->flagsC = value;
        }
        break;
    }
}

s32 Ovl11ChooseAreaSoundId(void);

void Ovl11RefreshAreaSoundBank(void) {
    s32 soundId;

    soundId = Ovl11ChooseAreaSoundId();
    if (!(((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x4000) && (((struct Ovl11SoundWork *) &D_8007AFF0)->selected != soundId)) {
        BeginMasterVolumeFadeOut(0x14);
        while (GetMasterVolumeRampState() != 0) {
            UpdateMasterVolumeAndSoundTransfer();
        }
        StopSoundSequence(0);
        OpenSoundSequencesAndStart();
        RewindSoundLoadSlots(2, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->previous = ((struct Ovl11SoundWork *) &D_8007AFF0)->header[7];
        QueueSoundLoadCommand((s16) soundId, 0);
        ((struct Ovl11SoundWork *) &D_8007AFF0)->selected = soundId;
        if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0xF80) {
            gOvl11AreaSoundRestartNeeded = 0;
        } else {
            gOvl11AreaSoundRestartNeeded = 1;
        }
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x80;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x800;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x100;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x200;
        ((struct Ovl11BCB5CState *) D_8006C838)->flags4450 &= ~0x400;
    }
}

void Ovl11ApplyAreaSoundVolume(void) {
    s16 primaryId;
    s32 hourClass;
    s32 volume;

    if (gOvl11AreaSoundRestartNeeded != 0) {
        StopSoundSequence(0);
    }
    volume = 0x64;
    hourClass = Ovl11ClassifyClockHour(((struct struct_8006C838_time *) D_8006C838)->field_44C0);
    if (((AreaSoundWork *) &D_8007AFF0)->record->flags & 0x1000) {
        volume = 0x50;
    }
    primaryId = ((AreaSoundWork *) &D_8007AFF0)->record->primaryId;
    if (primaryId == 0x37) {
        volume = 0x50;
    }
    if (primaryId == 0x43) {
        volume = 0x50;
    }
    if (hourClass == 3) {
        if ((primaryId != 0xB) && (primaryId != 0x33)) {
            volume /= 2;
        }
        if ((((AreaSoundStatus *) D_8006C838)->selector == 1) && !(((AreaSoundWork *) &D_8007AFF0)->record->flags & 0x1000)) {
            volume = 0x64;
        }
        if (((AreaSoundWork *) &D_8007AFF0)->record->secondaryId == 0xF) {
            volume = 0;
        }
        if (((struct struct_8006C838_button *) D_8006C838)->field_44BA == 3) {
            volume = 0;
        }
    }
    if (((AreaSoundWork *) &D_8007AFF0)->record->secondaryId == 0x28) {
        volume = 0x64;
    }
    if (gOvl11AreaSoundRestartNeeded != 0) {
        PlaySoundSequenceWithReverb(0, 0, ((AreaSoundWork *) &D_8007AFF0)->sequenceId, volume);
        return;
    }
    SetSoundSequenceSlotVolume(0, 0, 0, volume, 0xA);
}

s32 Ovl11ChooseAreaSoundId(void) {
    s16 status;
    u16 result;

    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x80) {
        return 0xF;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x800) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x100) {
        return 0xF;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x200) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags4450 & 0x400) {
        return 0xC;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->flags0C & 0x2000) {
        return 0x1F;
    }
    status = ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7];
    if (status == 0x1D) {
        return 0x1D;
    }
    if (status == 0xE) {
        return 0xE;
    }
    if ((u32) ((u16) ((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 - 3) < 2U) {
        return 0x17;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 1) {
        return 0x16;
    }
    if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 2) {
        return ((struct Ovl11BCB5CState *) D_8006C838)->field44BA + 1;
    }
    if (status == 0x18) {
        if (Ovl11ClassifyClockHour(((struct Ovl11BCB5CState *) D_8006C838)->field44C0) == 3) {
            return 0x18;
        }
        return 0x19;
    }
    if (Ovl11ClassifyClockHour(((struct Ovl11BCB5CState *) D_8006C838)->field44C0) == 3) {
        return 0x1C;
    }
    if (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7] == 0x3E7) {
        if (((struct Ovl11BCB5CState *) D_8006C838)->fieldE4C8 == 0) {
            return ((struct Ovl11BCB5CState *) D_8006C838)->field44BA + 1;
        }
    }
    result = (u16) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[7];
    if ((u32) (result - 0x1F) < 6U) {
        return (s16) result;
    }
    return 0x1B;
}

void ovl_11_func_800C9D38 (void);

/* Install projection, lighting, camera and scene pools before mode-specific
 * handlers. Retain their existing scratch-stack regions independently. */
void Ovl11ResetAreaScene(void) {
    u8 *objectState;
    struct Ovl11ResetState *state;

    Ovl11SetSceneProjection();
    Ovl11InitSceneLighting();
    Ovl11ResetSceneCameraOrbit();
    Ovl11InitVramAnimationSlots();
    Ovl11ResetSpriteRotations();
    Ovl11ClearSceneCameraVectors();
    if (gOvl11AreaEntryStep == 0xB) {
        Ovl11InstallAreaSelection(0, 0);
    } else {
        Ovl11InstallAreaSelection(1, gOvl11AreaReadStarted);
    }
    ovl_11_func_800C9D38();
    Ovl11ResetVertexBlendPool();
    Ovl11ClearSceneRecords();
    Ovl11InstallSceneRecords();
    Ovl11ResetChoiceMessages();
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 1) {
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        Ovl11BuildMapGrid(0);
        Ovl11ApplyMapTileTextures();
        SCRATCH_STACK_END();
    }
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 6) {
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        Ovl11BuildMode6SurfaceGrid(0);
        Ovl11RefreshMode6CellTextures();
        SCRATCH_STACK_END();
    }
    if (((((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37) || (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x43)) && (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 != 0x42)) {
        Ovl11BuildAreaGrid(0);
    }
    if ((u32) ((u16) ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 - 0xC) < 2U) {
        Ovl11ResetAreaGridState();
    }
    SetGameFlag(0x45U, 0U);
    Ovl11RefreshProgressStatus();
    Ovl11WeatherResetHook();
    state = (struct Ovl11ResetState *)D_8006C838;
    objectState = (u8 *)state + 0x51C8;
    state->field_51F0 = 0;
    gOvl11AreaReadStarted = 0;
    Ovl11InitObjectSpriteSlots(objectState);
    Ovl11InitSharedSpriteSlots(objectState);
    SetGameFlag(0x5EU, 0U);
    gOvl11HeldActionFallbackCounter = 0;
    state->field_51FE &= ~4;
}
