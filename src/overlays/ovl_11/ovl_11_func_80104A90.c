#include "common.h"
#include "psyq/memory.h"
#include "ovl11_selection_message.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_vertex_blend.h"
#include "message_window.h"
#include "ovl11_choice_messages.h"
#include "state_memory_init.h"
#include "ovl11_sprite_effect_pool.h"
#include "game_flags.h"
#include "ovl11_scene_record_pool.h"
#include "psyq/libgpu.h"
#include "ovl11_weather_effect.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_moving_items.h"
#include "psyq/libetc.h"
#include "psyq/abs.h"
#include "ovl11_progress_status.h"
#include "ovl11_primary_object_sprite_records.h"

void ovl_11_func_800D6628(void);
/* These two sprite-loader wrappers use their matched definition signatures;
 * the forwarded arg2/arg3 of func_80015704 are unused debug-hook parameters. */
s32 ovl_11_func_800CDE3C(u8 *arg0);
void ovl_11_func_800F6630(void);
void ovl_11_func_800F42A8(u16 arg0, s32 arg1);
void ovl_11_func_800FFA28(void);
void ovl_11_func_800CFD9C(void);
s32 ovl_11_func_800DE76C(void);
s32 ovl_11_func_80109310(s32 *arg0);
void ovl_11_func_800F1F48(s32 arg0);
void ovl_11_func_800FF96C(void);

void ovl_11_func_80104A90(void) {
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
    ovl_11_func_800D6628();
    Ovl11ResetWeatherParticles();
    Ovl11ResetAmbientTintFromTable();
    Ovl11SetWeatherParticleMode(base->field_E4C8);
    Ovl11InitAreaSpriteSource();
    Ovl11LoadIndexedSpriteSet(0);
    Ovl11LoadPrimaryObjectRecordSprites();
    Ovl11LoadFixedAreaSpriteBank();
    ovl_11_func_800CDE3C(base->field_51C8);
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
    ovl_11_func_800F1F48(gGraphicsHeapBase + 0x4290);
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
