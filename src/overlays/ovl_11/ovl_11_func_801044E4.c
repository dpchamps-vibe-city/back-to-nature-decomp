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
#include "ovl11_vertex_blend.h"
#include "message_window.h"
#include "ovl11_choice_messages.h"
#include "ovl11_daily_conditions.h"
#include "ovl11_sprite_effect_pool.h"
#include "ovl11_object_event_pool.h"
#include "ovl11_coordinate_clamp.h"
#include "ovl11_vram_animation_slots.h"
#include "ovl11_scene_record_pool.h"
#include "area_grid_triggers.h"
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

void func_80021D64 (void);
void ovl_11_func_80104A80 (void);
void ovl_11_func_801047FC (void);
void ovl_11_func_800DBFA0 (void);
void ovl_11_func_800D7B24 (void);
s32 ovl_11_func_800CDE3C (u8 *arg0);
void ovl_11_func_800F6630 (void);
void ovl_11_func_800F42A8 (u16 arg0, s32 arg1);
void ovl_11_func_8010C6A0 (void);
void ovl_11_func_80104898 (void);
void ovl_11_func_800FFA28 (void);
void ovl_11_func_80104A58 (void);
void ovl_11_func_80104A88 (void);
u16 *ovl_11_func_800CE744 (s32 arg0, s32 arg1);
void ovl_11_func_8010476C(void);

void ovl_11_func_801044E4(void) {
    Ovl11StartupView *base;
    Ovl11FarInitView *far;

    func_80021D64();
    base = (Ovl11StartupView *)D_8006C838;
    if (base->flags & 0x8000) {
        *base->reset = 0;
        gRandomState = 1;
    }
    ovl_11_func_80104A80();
    ovl_11_func_801047FC();
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
    ovl_11_func_800CDE3C(base->field_51C8);
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
    ovl_11_func_80104898();
    ovl_11_func_800FFA28();
    ovl_11_func_8010476C();
    Ovl11InitRecordFlagCounters();
    Ovl11StageFixedSpriteBank();
    Ovl11InitEventSchedules(gGraphicsHeapBase + 0x4290);
    far = (Ovl11FarInitView *)&D_8007AFF0;
    far->field_25498 = -1;
    base->field_4438 = -1;
    Ovl11ResetAreaGridState();
    ovl_11_func_80104A58();
    DealCardGameTable();
    Ovl11ApplyCurrentDayCondition();
    Ovl11ChooseNextDayCondition();
    ovl_11_func_80104A88();
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
