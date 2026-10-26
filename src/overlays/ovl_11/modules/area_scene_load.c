/*
 * area_scene_load — reconstructed translation unit.
 *
 * Area-scene preparation, state resets and entry/load/install phases,
 * carrying the three empty lifecycle hooks and reload wrapper.
 *
 * Original ovl_11:801044E4-80104EC8. Preparation owns the ordered reset
 * helpers; phase-table callbacks perform light entry, full load, installation
 * and reload. The empty lifecycle hooks remain compiled in original order.
 * Contract: ovl11_area_scene_load.h. Membership: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "card_deal.h"
#include "ovl11_selection_message.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_area_scene_load.h"
#include "screen_fade.h"
#include "sprite_rotation.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "projected_primitives.h"
#include "ovl11_vertex_blend.h"
#include "message_window.h"
#include "scene_selection.h"
#include "sound_commands.h"
#include "ovl11_choice_messages.h"
#include "ovl11_daily_conditions.h"
#include "state_memory_init.h"
#include "ovl11_sprite_effect_pool.h"
#include "ovl11_object_event_pool.h"
#include "ovl11_coordinate_clamp.h"
#include "ovl11_vram_animation_slots.h"
#include "random.h"
#include "game_flags.h"
#include "ovl11_scene_record_pool.h"
#include "area_grid_triggers.h"
#include "ovl11_area_mode_sequence.h"
#include "ovl11_weather_effect.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_moving_items.h"
#include "psyq/abs.h"
#include "ovl11_progress_status.h"
#include "ovl11_record_flag_counters.h"
#include "ovl11_primary_object_sprite_records.h"
#include "ovl11_record_slot_table.h"
#include "ovl11_scene_setup_install.h"
#include "ovl11_pending_event_queue.h"
#include "ovl11_record_pool_lifecycle.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_event_schedule.h"
#include "ovl11_primary_object_sprite_slots.h"

void func_80021D64 (void);
void Ovl11AreaResetBeginHook (void);
void Ovl11ResetAreaSlotRecords (void);
void ovl_11_func_800DBFA0 (void);
void ovl_11_func_800D7B24 (void);
void ovl_11_func_800F6630 (void);
void ovl_11_func_800F42A8 (u16 arg0, s32 arg1);
void ovl_11_func_8010C6A0 (void);
void Ovl11AreaResetEffectsHook (void);
void ovl_11_func_800FFA28 (void);
void Ovl11ClearAreaSpriteWork (void);
void Ovl11AreaResetEndHook (void);
u16 *ovl_11_func_800CE744 (s32 arg0, s32 arg1);
void Ovl11ClearAreaRowRecords(void);

void Ovl11PrepareAreaScene(void) {
    Ovl11StartupView *base;
    Ovl11FarInitView *far;

    func_80021D64();
    base = (Ovl11StartupView *)D_8006C838;
    if (base->flags & 0x8000) {
        *base->reset = 0;
        gRandomState = 1;
    }
    Ovl11AreaResetBeginHook();
    Ovl11ResetAreaSlotRecords();
    Ovl11ResetSlotSpriteCache();
    Ovl11InitRecordSlotTable();
    Ovl11InitClockAndCalendar();
    Ovl11InitPrimaryObjectRecordSprites();
    Ovl11ResetAllEventSlots();
    Ovl11AllocateAllEventSlots();
    Ovl11ResetWeatherParticles();
    Ovl11ResetAmbientTintFromTable();
    ovl_11_func_800DBFA0();
    Ovl11ResetCoordinateClamp();
    Ovl11InitVramAnimationSlots();
    Ovl11SetSceneCollisionTolerance();
    ovl_11_func_800D7B24();
    Ovl11ClearResourceIndex();
    Ovl11AreaSpriteBankHook();
    Ovl11LoadFixedAreaSpriteBank();
    Ovl11InitObjectSpriteSlots(base->field_51C8);
    Ovl11LoadOffsetSpriteSet((s32) base->field_44BA);
    Ovl11ClearSceneRecords();
    Ovl11ResetVertexBlendPool();
    ovl_11_func_800F6630();
    ovl_11_func_800F42A8((u16) base->field_44BA, 0);
    Ovl11ResetRecordPools();
    Ovl11ResetMovingItemRecords();
    Ovl11ResetChoiceMessages();
    ovl_11_func_800F6630();
    Ovl11ResetSelectionMessage();
    ovl_11_func_8010C6A0();
    ResetMessageWindow();
    Ovl11ResetProgressControl();
    Ovl11ResetEventQueues();
    Ovl11ClearSpriteEffects();
    Ovl11AreaResetEffectsHook();
    ovl_11_func_800FFA28();
    Ovl11ClearAreaRowRecords();
    Ovl11InitRecordFlagCounters();
    Ovl11StageFixedSpriteBank();
    Ovl11InitEventSchedules(gGraphicsHeapBase + 0x4290);
    far = (Ovl11FarInitView *)&D_8007AFF0;
    far->field_25498 = -1;
    base->field_4438 = -1;
    Ovl11ResetAreaGridState();
    Ovl11ClearAreaSpriteWork();
    DealCardGameTable();
    Ovl11ApplyCurrentDayCondition();
    Ovl11ChooseNextDayCondition();
    Ovl11AreaResetEndHook();
    if (base->flags & 0x8000) {
        ovl_11_func_800CE744(0x162, -1);
        ovl_11_func_800CE744(0x160, -1);
        ovl_11_func_800CE744(0x161, -1);
        ovl_11_func_800CE744(0x162, -1);
        ovl_11_func_800CE744(0x164, -1);
        ovl_11_func_800CE744(0x165, -1);
        ovl_11_func_800CE744(0x10A, -1);
        ovl_11_func_800CE744(0x109, -1);
        ovl_11_func_800CE744(0x109, -1);
        ovl_11_func_800CE744(0x10A, -1);
        ovl_11_func_800CE744(0x109, -1);
        base->field_4502 = 0x32;
        base->field_4508 = 0x32;
    }
}

/* Clear three rows of six 14-byte records: first halfword is -1 and the
 * remaining six are zero. The two bases reach the same record array, with
 * pX addressing its last halfword. */
void Ovl11ClearAreaRowRecords(void) {
    volatile s16 *pW;
    volatile s16 *pX;
    s16 *pBase;
    s16 *base2;
    u32 rows;
    u32 n;
    u32 next;
    u32 rb;

    rows = 0;
    pBase = (s16 *)D_80074838;
    base2 = (s16 *)&D_8006C838;
    base2 += 0x7291;
    do {
        n = 0;
        next = rows + 1;
        pX = (s16 *)(rows * 0x54 + (u32)base2);
        rb = rows * 0x54 + 0x6520;
        pW = (s16 *)(rb + (u32)pBase);
        do {
            pW[-5] = -1;
            pW[-4] = 0;
            pW[-3] = 0;
            pW[-2] = 0;
            pW[-1] = 0;
            pW[0] = 0;
            *pX = 0;
            pX += 7;
            pW += 7;
            n++;
        } while (n < 6U);
        rows = next;
    } while (rows < 3U);
}

typedef struct {
    /* 0x000 */ char pad_000[0x0C];
    /* 0x00C */ s32 field_0C;
    /* 0x010 */ char pad_010[0x4428];
    /* 0x4438 */ s16 field_4438;
    /* 0x443A */ char pad_43A[0x94];
    /* 0x44CE */ s16 field_44CE;
    /* 0x44D0 */ s16 field_44D0;
    /* 0x44D2 */ s16 field_44D2;
    /* 0x44D4 */ s16 field_44D4;
    /* 0x44D6 */ char pad_44D6[0x2A];
    /* 0x4500 */ s16 field_4500;
    /* 0x4502 */ s16 field_4502;
    /* 0x4504 */ char pad_4504[0x4];
    /* 0x4508 */ s16 field_4508;
    /* 0x450A */ char pad_450A[0x9FC8];
    /* 0xE4D2 */ s16 field_E4D2;
    /* 0xE4D4 */ char pad_E4D4[0x4];
    /* 0xE4D8 */ s16 recs[5][6];
} AreaSlotResetView;

void Ovl11ResetAreaSlotRecords(void) {
    AreaSlotResetView *v;
    char *far_base;
    u32 i;

    v = (AreaSlotResetView *)&D_8006C838;
    v->field_E4D2 = 1;
    v->field_44CE = 0;
    v->field_44D0 = 0;
    v->field_44D2 = 0;
    if (v->field_0C & 0x8000) {
        v->field_44CE = 0;
        v->field_44D0 = 0;
        v->field_44D2 = 0;
    }
    v->field_44D4 = 0;
    v->field_4500 = 0;
    v->field_4502 = 0;
    v->field_4508 = 0;
    far_base = (char *)&D_8007AFF0;
    *(s32 *)(far_base + 0x2549C) = -1;
    v->field_4438 = -1;
    for (i = 0; i < 5; i++) {
        v->recs[i][0] = -1;
        v->recs[i][4] = -1;
        v->recs[i][5] = -1;
    }
}

void Ovl11AreaResetEffectsHook(void) {
}

void ovl_11_func_800F6630(void);

typedef struct {
    char pad_000[0xC];
    s32 field_0C;
    char pad_010[0x4438 - 0x10];
    s16 field_4438;
    char pad_443A[0x4450 - 0x443A];
    s32 field_4450;
    char pad_4454[0x4488 - 0x4454];
    s32 field_4488;
    char pad_448C[0xE4C8 - 0x448C];
    s16 field_E4C8;
} AreaLightEntryView;

void Ovl11BeginLightAreaEntry(void) {
    AreaLightEntryView *base;
    char *far;

    base = (AreaLightEntryView *)&D_8006C838;
    far = (char *)&D_8007AFF0;

    base->field_0C &= 0xFF7FFFFF;
    *(s32 *)(far + 0x25484) = 0xFFFF;
    *(s32 *)(far + 0x25488) = 0xFFFF;

    Ovl11AreaSpriteBankHook();
    Ovl11ResetWeatherParticles();
    Ovl11ResetAmbientTintFromTable();
    Ovl11SetWeatherParticleMode(base->field_E4C8);
    Ovl11ResetMovingItemRecords();
    Ovl11ResetChoiceMessages();
    ovl_11_func_800F6630();

    base->field_4488 = 1;
    gOvl11AreaEntryStep = 10;
    *(s32 *)(far + 0x2548C) = 0;
    *(s16 *)(far + 0x247A8) = -1;
    *(s16 *)(far + 0x247A6) = -1;
    *(s16 *)(far + 0x2479C) = -1;
    *(s32 *)(far + 0x25498) = -1;
    *(s32 *)(far + 0x25390) = 0;
    *(s32 *)(far + 0x2549C) = -1;
    base->field_4438 = -1;
    QueueSoundLoadCommand(1000, 0);

    base->field_4450 &= ~0xF80;
    if (GetPreviousSceneId() == 3) {
        base->field_4450 |= 0x80;
        Ovl11SelectCurrentHourTint();
    }
    if (GetPreviousSceneId() == 4) {
        base->field_4450 |= 0x100;
        Ovl11SelectCurrentHourTint();
    }
    if (GetPreviousSceneId() == 5) {
        base->field_4450 |= 0x200;
        Ovl11SelectCurrentHourTint();
    }
    if (GetPreviousSceneId() == 0xB) {
        base->field_4450 |= 0x400;
        Ovl11SelectCurrentHourTint();
    }
    if (GetPreviousSceneId() == 0xC) {
        base->field_4450 |= 0x800;
        Ovl11SelectCurrentHourTint();
    }
}

void Ovl11ClearAreaSpriteWork(void) {
    u32 wordIndex;
    s32 *word;

    word = &gAreaSpriteWorkBase;
    for (wordIndex = 0; wordIndex < 0x680; wordIndex++) {
        *word = 0;
        word++;
    }
}

void Ovl11AreaResetBeginHook(void) {
}

void Ovl11AreaResetEndHook(void) {
}

/* These two sprite-loader wrappers use their matched definition signatures;
 * the forwarded arg2/arg3 of func_80015704 are unused debug-hook parameters. */
void ovl_11_func_800F42A8(u16 arg0, s32 arg1);
void ovl_11_func_800FFA28(void);
void ovl_11_func_800CFD9C(void);
s32 ovl_11_func_800DE76C(void);
s32 ovl_11_func_80109310(s32 *arg0);
void ovl_11_func_800FF96C(void);

void Ovl11LoadAreaScene(void) {
    Ovl11AreaInitView *base;
    Ovl11FarInitView *far;
    s32 value;

    far = (Ovl11FarInitView *)&D_8007AFF0;
    far->field_25484 = 0xFFFF;
    far->field_25488 = 0xFFFF;
    far->field_247A8 = -1;
    far->field_247A6 = -1;
    far->field_2479C = -1;
    far->buffer = &gAreaSetupBuffer;
    far->field_25498 = -1;
    far->field_25390 = 0;
    far->field_2549C = -1;
    far->data = gLoadedAreaHeaderCopy;
    base = (Ovl11AreaInitView *)D_8006C838;
    base->flags = (s32) (base->flags & 0xFF7FFFFF);
    value = base->field_44CE * 4;
    value += 0x50;
    far->field_2547C = base->field_44BA + value;
    memset(&gAreaSetupBuffer, 0, 0x10);
    base->field_5214 = 0;
    BindStateDataTables(1);
    Ovl11AreaSpriteBankHook();
    Ovl11ResetSlotSpriteCache();
    Ovl11ResetWeatherParticles();
    Ovl11ResetAmbientTintFromTable();
    Ovl11SetWeatherParticleMode(base->field_E4C8);
    Ovl11InitAreaSpriteSource();
    Ovl11LoadIndexedSpriteSet(0);
    Ovl11LoadPrimaryObjectRecordSprites();
    Ovl11LoadFixedAreaSpriteBank();
    Ovl11InitObjectSpriteSlots(base->field_51C8);
    Ovl11LoadOffsetSpriteSet((s32) base->field_44BA);
    Ovl11ClearSceneRecords();
    Ovl11ResetVertexBlendPool();
    ovl_11_func_800F6630();
    ovl_11_func_800F42A8((u16) base->field_44BA, 1);
    Ovl11ResetClockEvents();
    Ovl11ResetMovingItemRecords();
    Ovl11ResetChoiceMessages();
    ovl_11_func_800F6630();
    Ovl11ResetSelectionMessage();
    ResetMessageWindow();
    Ovl11RefreshProgressStatusEntry();
    Ovl11ClearSpriteEffects();
    ovl_11_func_800FFA28();
    ovl_11_func_800CFD9C();
    ovl_11_func_800DE76C();
    ovl_11_func_80109310((s32 *) &D_800759E4);
    Ovl11StageFixedSpriteBank();
    Ovl11InitEventSchedules(gGraphicsHeapBase + 0x4290);
    base->field_4488 = 1;
    gOvl11AreaEntryStep = 0xA;
    far->field_2548C = 0;
    base->field_4438 = -1;
    far->field_254A0 = 0xC;
    base->field_4450 = (s32) (base->field_4450 | 0x40);
    if (base->field_44C0 >= 7) {
        SetGameFlag(0xADU, 1U);
    }
    ovl_11_func_800FF96C();
}

void func_8001E340(Ovl11SceneGrid **arg0);
void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
void ovl_11_func_800CBB9C();

void Ovl11InstallAreaScene(void) {
    ((Ovl11AreaInitView *)D_8006C838)->flags &= 0xFF7FFFFF;
    Ovl11ResetWeatherParticles();
    Ovl11ResetAmbientTintFromTable();
    Ovl11SetWeatherParticleMode(((Ovl11AreaInitView *)D_8006C838)->field_E4C8);
    Ovl11SelectCurrentHourTint();
    Ovl11ResetMovingItemRecords();
    ((Ovl11AreaInitView *)D_8006C838)->field_4488 = 2;
    ((Ovl11FarInitView *)&D_8007AFF0)->field_2548C = 0;
    InitProjectedPrimitiveSource(gOvl11SceneGeometry, (u8 *)D_8007AFF0 + 0x2000);
    Ovl11SetCoordinateClamp(&gLoadedAreaClampFirstCorner, &gLoadedAreaClampSecondCorner);
    func_8001E340((Ovl11SceneGrid **)gOvl11SceneGeometry);
    Ovl11InitVramAnimationSlots();
    Ovl11ResetVertexBlendPool();
    Ovl11ClearSceneRecords();
    Ovl11InstallSceneRecords();
    Ovl11RequestVramAnimations(gLoadedAreaHeaderCopy);
    Ovl11RegisterPoolSpriteRotations((s32)gLoadedAreaHeaderCopy);
    Ovl11RefreshAreaSoundBank();
    while (IsSoundTransferComplete() == 0) {
        UpdateMasterVolumeAndSoundTransfer();
    }
    OpenSoundSequencesAndStart();
    Ovl11ApplyAreaSoundVolume();
    if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == 2) {
        ovl_11_func_800F0FB0(8);
        ovl_11_func_800F0F50();
        Ovl11ServiceActiveRecordSlot();
        Ovl11ServiceActiveRecordSlot();
    }
    if (!(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 4) && !GetGameFlag(2)) {
        ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11AreaInitView *)D_8006C838)->field_51C8, Rand(4) + 0x11B, 0);
        if (((Ovl11AreaInitView *)D_8006C838)->field_44C0 >= 7) {
            SetGameFlag(0xAD, 1);
        }
    }
    BeginMasterVolumeFadeIn(10);
    BeginScreenFadeDecrease(10, 0, 2);
    ((Ovl11AreaInitView *)D_8006C838)->flags &= 0xFF7FFFFF;
}

void Ovl11LoadAreaScene(void);

void Ovl11ReloadAreaScene(void) {
    Ovl11LoadAreaScene();
}
