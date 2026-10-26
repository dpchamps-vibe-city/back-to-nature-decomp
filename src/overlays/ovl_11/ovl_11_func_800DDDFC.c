#include "common.h"
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

void ovl_11_func_800D7B8C (void);
void ovl_11_func_800CECB0 (void);
void ovl_11_func_800CCCC0 (void);
void ovl_11_func_800DBFEC (u16 arg0, u16 arg1, s32 arg2);
void ovl_11_func_800CD0C0 (s32 *arg0);
void ovl_11_func_800CCC5C (void);
void ovl_11_func_800EEECC (void);
void ovl_11_func_800F1F48 (s32 arg0);
void ovl_11_func_800DBC4C (void);
void ovl_11_func_8010C9B4 (void);
void ovl_11_func_800F40A4 (void);
void ovl_11_func_800F3D40 (void);
void ovl_11_func_800F2620 (void);
s32 ovl_11_func_800DE76C (void);
void ovl_11_func_800F4D28 (void);
void ovl_11_func_800DE4C0 (void);
void ovl_11_func_800F4BB4(void);

s32 ovl_11_func_800DDDFC(void) {
    s32 result;
    s32 id;
    s32 offset;
    s32 same;
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
        ovl_11_func_800D7B8C();
        ovl_11_func_800CECB0();
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
            same = ((Ovl11SetupWorkView *)&D_8007AFF0)->id == id;
            ((Ovl11SetupWorkView *)&D_8007AFF0)->id = id;
            ovl_11_func_800DBFEC((u16)((Ovl11SetupWorkView *)&D_8007AFF0)->id, 0x8000, same);
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
            ovl_11_func_800CD0C0(((Ovl11DDDFCState *)D_8006C838)->position);
            Ovl11ResetMovingItemRecords();
            ((Ovl11DDDFCState *)D_8006C838)->status = (((Ovl11DDDFCState *)D_8006C838)->status | 0x4000) & 0xFFF07FFF;
            ovl_11_func_800CCC5C();
            ovl_11_func_800EEECC();
            Ovl11StageFixedSpriteBank();
            ovl_11_func_800F1F48(gGraphicsHeapBase + 0x4290);
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
        ovl_11_func_800F4BB4();
        ovl_11_func_800DBC4C();
        ovl_11_func_8010C9B4();
        ovl_11_func_800F40A4();
        ovl_11_func_800F3D40();
        ovl_11_func_800F2620();
        ovl_11_func_800DE76C();
        ovl_11_func_800F4D28();
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
            ovl_11_func_800DE4C0();
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
